"""Run under sourced ROS in an isolated ROS_DOMAIN_ID; starts MOCK only."""
import importlib.util
import json
import sys
import os
from pathlib import Path
import subprocess
import threading
import time
import uuid

import pytest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('robot_client', ROOT/'scripts/robot_client.py')
client = importlib.util.module_from_spec(spec)
spec.loader.exec_module(client)


def eventually(fn, timeout=8):
    deadline=time.monotonic()+timeout
    while time.monotonic()<deadline:
        try:
            value=fn()
            if value:
                return value
        except RuntimeError:
            pass
        time.sleep(0.05)
    raise AssertionError('condition did not become true')


@pytest.fixture(scope='module')
def rig(tmp_path_factory):
    # Never run these tests against the default domain or an existing robot.
    assert os.environ.get('ROS_DOMAIN_ID') == '187', 'Use ROS_DOMAIN_ID=187 for isolated mock tests'
    import rclpy
    from sensor_msgs.msg import Image
    from rclpy.qos import qos_profile_sensor_data
    tmp=tmp_path_factory.mktemp('recovery')
    logs=[]
    processes=[]
    for name, command in [
        ('execution', ['ros2','run','robot_execution','execution_node','--ros-args','-p','backend:=mock']),
        ('observer', ['ros2','run','insight_camera','observation_node','--ros-args',
                      '-p','rgb_transport:=raw','-p','rgb_topic:=/test/rgb',
                      '-p','left_topic:=/test/left','-p','right_topic:=/test/right',
                      '-p',f'output_dir:={tmp}/images','-p','capture_timeout:=0.4'])]:
        log=open(tmp/(name+'.log'),'w');logs.append(log)
        processes.append(subprocess.Popen(command,stdout=log,stderr=subprocess.STDOUT))
    bridge=client.Bridge(tmp/'journal')
    publisher=rclpy.create_node('test_cameras')
    streams={name:publisher.create_publisher(Image,'/test/'+name,qos_profile_sensor_data)
             for name in ('rgb','left','right')}
    enabled={'rgb','left','right'}
    done=threading.Event()
    def frames():
        while not done.wait(0.03):
            stamp=publisher.get_clock().now().to_msg()
            for name,pub in streams.items():
                if name not in enabled:
                    continue
                image=Image();image.header.stamp=stamp;image.header.frame_id=name
                if name=='right' and 'stale_right' in enabled:
                    image.header.stamp=rclpy.time.Time(nanoseconds=publisher.get_clock().now().nanoseconds-2_000_000_000).to_msg()
                image.width=64;image.height=48
                image.encoding='bgr8' if name=='rgb' else 'mono8'
                image.step=64*(3 if name=='rgb' else 1)
                image.data=bytes([60])*(image.step*48)
                pub.publish(image)
    thread=threading.Thread(target=frames);thread.start()
    eventually(lambda: bridge.state()['state_valid'])
    eventually(lambda: bridge.capture.service_is_ready())
    yield bridge, enabled, tmp
    done.set();thread.join();publisher.destroy_node()
    bridge.executor.shutdown();bridge.thread.join();bridge.node.destroy_node();rclpy.shutdown()
    for process in processes:
        process.terminate()
        try:process.wait(5)
        except subprocess.TimeoutExpired:process.kill();process.wait()
    for log in logs:log.close()


def goal(x=0.4):
    return {'command_id':str(uuid.uuid4()),'kind':'move_to','timeout_sec':3.0,
            'target':{'header':{'frame_id':'base_link'},'pose':{
                'position':{'x':x,'y':0.0,'z':0.3},
                'orientation':{'x':0.0,'y':0.0,'z':0.0,'w':1.0}}}}


def set_error(bridge, error):
    from rcl_interfaces.srv import SetParameters
    from rcl_interfaces.msg import Parameter, ParameterValue, ParameterType
    endpoint=bridge.node.create_client(SetParameters,'/robot_execution/set_parameters')
    request=SetParameters.Request(parameters=[Parameter(name='mock_error',value=ParameterValue(
        type=ParameterType.PARAMETER_STRING,string_value=error))])
    assert bridge.service(endpoint,request).results[0].successful
    bridge.node.destroy_client(endpoint)


def observe(bridge):
    snapshot=bridge.handle({'op':'observe'})
    assert snapshot['code']=='observed', snapshot
    return snapshot


def test_workspace_preflight_and_large_step(rig):
    bridge,_,_=rig
    bad=goal(1.2)
    check=bridge.handle({'op':'check','goal':bad})
    assert not check['valid'] and check['code']=='outside_workspace'
    assert 'x requested=' in check['message']
    assert not check['ik_checked'] and check['state']['motion_allowed']
    assert check['violations'][0]['axis']=='x'
    ik=bridge.handle({'op':'check','check_ik':True,'goal':goal()})
    assert ik['valid'] and ik['ik_checked'] and not ik['path_checked']
    assert not bridge.state()['busy']
    observe(bridge)
    rejected=bridge.handle({'op':'execute','goal':bad})
    assert rejected['result']['code']=='outside_workspace'
    assert bridge.state()['software_motion_enabled']
    accepted=bridge.handle({'op':'execute','goal':goal(-0.4)})
    assert accepted['result']['success']  # 0.7 m exceeds old fixed step limit.
    assert accepted['observation']['observation_valid']
    snapshot=accepted['observation']
    assert all(bridge.stamp_ns(pair['state'])>=snapshot['after_ns'] for pair in snapshot['associated_states'])


def test_async_ik_then_new_target_and_duplicate_does_not_move(rig):
    bridge,_,_=rig
    observe(bridge);set_error(bridge,'ik')
    failed_goal=goal(0.45)
    failed=bridge.handle({'op':'execute','goal':failed_goal})
    assert failed['result']['code']=='ik_rejected', failed
    state=failed['result']['state']
    assert state['servo_enabled'] and state['software_motion_enabled']
    assert state['fault']=='' and state['stationary'] and not state['busy']
    set_error(bridge,'')
    new_goal=goal(0.35)
    success=bridge.handle({'op':'execute','goal':new_goal})
    assert success['result']['success'], success
    queried=bridge.handle({'op':'query','command_id':new_goal['command_id']})
    assert queried['found'] and queried['phase']=='finished'
    duplicate=bridge.handle({'op':'execute','goal':failed_goal})
    assert duplicate['cached']['result']['code']=='ik_rejected'
    assert abs(bridge.state()['flange_pose']['position']['x']-0.35)<1e-9



def test_late_duplicate_ik_events_while_idle_do_not_poison_new_action(rig):
    bridge,_,_=rig
    from std_srvs.srv import Trigger
    endpoint=bridge.node.create_client(Trigger,'/robot/mock_ik_event')
    for _ in range(3):
        assert bridge.service(endpoint,Trigger.Request()).success
    bridge.node.destroy_client(endpoint)
    observe(bridge)
    result=bridge.handle({'op':'execute','goal':goal(0.36)})
    assert result['result']['success'], result
    assert result['result']['state']['fault']==''

def test_missing_camera_retries_observation_without_motion(rig):
    bridge,enabled,_=rig
    enabled.remove('right');time.sleep(0.1)
    snapshot=bridge.handle({'op':'observe','after_ns':bridge.node.get_clock().now().nanoseconds})
    assert snapshot['code']=='observation_unavailable'
    assert 'observation_timeout' in snapshot['details']
    blocked=bridge.handle({'op':'execute','goal':goal()})
    assert blocked['code']=='observation_required'
    enabled.add('right');observe(bridge)



def test_old_camera_source_stamp_is_not_hidden_by_fresh_arrival(rig):
    bridge,enabled,_=rig
    enabled.add('stale_right')
    try:
        time.sleep(0.1)
        snapshot=bridge.handle({'op':'observe','after_ns':bridge.node.get_clock().now().nanoseconds})
        assert snapshot['code']=='observation_unavailable'
        details=json.loads(snapshot['details'])
        assert details['frame_counts']['right']>0
        assert details['newest_stamp_ns']['right']<details['after_ns']
    finally:
        enabled.remove('stale_right')
    observe(bridge)

def test_uncertain_submission_blocks_new_id(rig):
    bridge,_,_=rig
    observe(bridge)
    path=bridge.journal/'unknown.json'
    client.atomic_json(path,{'goal':goal(), 'phase':'submission_pending'})
    try:
        result=bridge.handle({'op':'execute','goal':goal()})
        assert result['code']=='unresolved_previous_action'
        assert not result['query']['found']
    finally:
        path.unlink()



def test_uncertain_acknowledgement_is_monitored_and_timeout_remains_unknown(rig):
    bridge,_,_=rig
    observe(bridge);set_error(bridge,'ack_unknown')
    result=bridge.handle({'op':'execute','goal':goal(0.38)})
    assert result['result']['success'], result
    set_error(bridge,'ack_unknown_timeout')
    request=goal(0.4);request['timeout_sec']=0.5
    result=bridge.handle({'op':'execute','goal':request})
    assert result['result']['code']=='result_unknown', result
    assert not result['result']['state']['software_motion_enabled']
    set_error(bridge,'')
    from std_srvs.srv import Trigger
    endpoint=bridge.node.create_client(Trigger,'/robot/enable')
    assert bridge.service(endpoint,Trigger.Request()).success
    bridge.node.destroy_client(endpoint)
    eventually(lambda: bridge.state()['motion_allowed'])


def test_socket_client_and_record_mock_timing(rig):
    bridge,_,tmp=rig
    socket_path=tmp/'client.sock'
    log=open(tmp/'client.log','w')
    process=subprocess.Popen([sys.executable,str(ROOT/'scripts/robot_client.py'),
        '--socket',str(socket_path),'serve','--journal',str(tmp/'socket_journal')],
        stdout=log,stderr=subprocess.STDOUT)
    def call(request):
        run=subprocess.run([sys.executable,str(ROOT/'scripts/robot_client.py'),
            '--socket',str(socket_path),'call',json.dumps(request)],capture_output=True,text=True,timeout=10)
        assert run.returncode==0, run.stderr
        return json.loads(run.stdout)
    try:
        eventually(lambda: socket_path.exists())
        eventually(lambda: call({'op':'state'}).get('state_valid'))
        detail=call({'op':'observe','roi':[2,2,10,10]})
        assert detail['code']=='observed' and Path(detail['detail_view']['file']).exists()
        request={'op':'execute','goal':goal(0.37)}
        result=call(request)
        assert result['result']['success'],result
        assert result['observation']['images']
        assert 'associated_states' not in result['observation']
        assert call(request)['cached']['result']['success']
        changed=json.loads(json.dumps(request));changed['goal']['target']['pose']['position']['x']=0.42
        assert call(changed)['code']=='command_id_conflict'
        fast=[];legacy=[]
        for _ in range(3):
            start=time.monotonic();call({'op':'state'});fast.append(time.monotonic()-start)
            start=time.monotonic()
            old=subprocess.run(['ros2','topic','echo','/robot/state','--once'],
                capture_output=True,text=True,timeout=10)
            assert old.returncode==0
            legacy.append(time.monotonic()-start)
        metrics={'environment':'mock robot, synthetic 64x48 RGB/stereo, ROS_DOMAIN_ID=187',
                 'client_state_call_sec':fast, 'ros2_topic_echo_once_sec':legacy,
                 'sample_cycle_total_sec':result['cycle_total_sec'],
                 'sample_observation_timings':result['observation']['timings'],
                 'limitations':'These are local mock measurements, not real robot or camera speedups.'}
        output=ROOT/'test-results/recovery_metrics.json'
        output.parent.mkdir(exist_ok=True)
        output.write_text(json.dumps(metrics,indent=2)+'\n')
    finally:
        process.terminate()
        try:process.wait(5)
        except subprocess.TimeoutExpired:process.kill();process.wait()
        log.close()
    assert not socket_path.exists(), 'Graceful shutdown should remove its own socket'

def test_real_fault_wins_over_simultaneous_ik(rig):
    bridge,_,_=rig
    observe(bridge);set_error(bridge,'ik_and_hard')
    result=bridge.handle({'op':'execute','goal':goal()})
    assert result['result']['code']=='state_fault',result
    assert '9000' in result['result']['state']['fault']
    assert not result['result']['state']['software_motion_enabled']
