#!/usr/bin/env python3
"""Persistent, local-only ROS bridge. No automatic enable, restart or motion retry."""
import argparse
from collections import deque
import json
import os
from pathlib import Path
import socket
import socketserver
import threading
import time
import uuid


def default_socket():
    return str(Path(os.environ.get('XDG_RUNTIME_DIR', '/tmp')) / f'astra-{os.getuid()}.sock')


def atomic_json(path, value, durable=False):
    temp = path.with_suffix('.tmp')
    with temp.open('w') as stream:
        stream.write(json.dumps(value, ensure_ascii=False, allow_nan=False) + '\n')
        if durable:
            stream.flush()
            os.fsync(stream.fileno())
    os.replace(temp, path)
    if durable:
        descriptor=os.open(path.parent, os.O_DIRECTORY)
        try:os.fsync(descriptor)
        finally:os.close(descriptor)


class Bridge:
    def __init__(self, journal):
        import rclpy
        from rclpy.action import ActionClient
        from rclpy.executors import SingleThreadedExecutor
        from robot_interfaces.action import ExecuteStep
        from robot_interfaces.msg import RobotState
        from robot_interfaces.srv import CheckStep, QueryStep, CaptureObservation
        from rosidl_runtime_py.convert import message_to_ordereddict
        self.convert = lambda msg: dict(message_to_ordereddict(msg))
        self.Step, self.Check, self.Query, self.Capture = ExecuteStep, CheckStep, QueryStep, CaptureObservation
        rclpy.init()
        self.node = rclpy.create_node('astra_client')
        self.states = deque(maxlen=200)
        self.state_lock = threading.Lock()
        self.operation_lock = threading.Lock()
        self.latest_observation = None
        self.node.create_subscription(RobotState, '/robot/state', self.receive, 10)
        self.action = ActionClient(self.node, ExecuteStep, '/robot/execute_step')
        self.check = self.node.create_client(CheckStep, '/robot/check_step')
        self.query = self.node.create_client(QueryStep, '/robot/query_step')
        self.capture = self.node.create_client(CaptureObservation, '/insight_observer/capture_after')
        self.journal = Path(journal).expanduser()
        self.journal.mkdir(parents=True, exist_ok=True, mode=0o700)
        import fcntl
        self.journal_lock = (self.journal / '.lock').open('a')
        fcntl.flock(self.journal_lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
        self.executor = SingleThreadedExecutor()
        self.executor.add_node(self.node)
        self.thread = threading.Thread(target=self.executor.spin, daemon=True)
        self.thread.start()

    def receive(self, msg):
        with self.state_lock:
            self.states.append((time.monotonic(), self.convert(msg)))

    def state(self):
        with self.state_lock:
            if not self.states:
                raise RuntimeError('state_unavailable')
            arrived, state = self.states[-1]
        if time.monotonic() - arrived > 0.5:
            raise RuntimeError('state_stale')
        return state

    @staticmethod
    def wait(future, timeout):
        event = threading.Event()
        future.add_done_callback(lambda _: event.set())
        if not event.wait(timeout):
            raise TimeoutError('response_timeout; execution may still be in progress; query command_id')
        return future.result()

    def service(self, client, req, timeout=3):
        if not client.wait_for_service(timeout_sec=1):
            raise RuntimeError('service_unavailable')
        return self.wait(client.call_async(req), timeout)

    def goal(self, data):
        from rosidl_runtime_py.set_message import set_message_fields
        goal = self.Step.Goal()
        set_message_fields(goal, data)
        return goal

    def query_id(self, command_id):
        req = self.Query.Request(command_id=command_id)
        result = self.convert(self.service(self.query, req))
        if not result['found']:
            import hashlib
            path = self.journal / (hashlib.sha256(command_id.encode()).hexdigest()+'.json')
            if path.exists():
                saved = json.loads(path.read_text())
                result['journal_phase'] = saved['phase']
                if saved['phase'] == 'finished':
                    result['historical_result'] = saved['result']
        return result

    @staticmethod
    def stamp_ns(state):
        stamp = state['header']['stamp']
        return stamp['sec'] * 10**9 + stamp['nanosec']

    def observe(self, after_ns=0, previous=None):
        self.latest_observation = None
        started = time.monotonic()
        state = self.state()
        if not state['state_valid'] or state['busy'] or not state['stationary']:
            raise RuntimeError('observation_requires_fresh_stationary_state')
        req = self.Capture.Request()
        req.after.sec, req.after.nanosec = divmod(after_ns, 10**9)
        captured = self.service(self.capture, req, 8)
        if not captured.success:
            return {'code': 'observation_unavailable', 'details': captured.message,
                    'previous_action_result': previous, 'robot_state': self.state()}
        path = Path(captured.metadata_path)
        metadata = json.loads(path.read_text())
        # State-to-image association is explicit; receipt time is not a source stamp.
        with self.state_lock:
            candidates = [item for item in self.states if self.stamp_ns(item[1]) >= after_ns]
        if not candidates:
            return {'code': 'observation_unaligned', 'metadata_path': str(path),
                    'message': 'No robot state sample after the action boundary'}
        paired = []
        for view in metadata['views']:
            _, closest = min(candidates, key=lambda item: abs(self.stamp_ns(item[1])-view.get('association_stamp_ns', view['stamp_ns'])))
            delta = abs(self.stamp_ns(closest)-view.get('association_stamp_ns', view['stamp_ns']))/1e9
            paired.append({'role': view['role'], 'state': closest, 'state_delta_sec': delta})
        current = self.state()
        coherent = all(p['state_delta_sec'] <= 0.15 and p['state']['state_valid'] and
                       p['state']['stationary'] and not p['state']['busy'] and
                       max(p['state']['pose_age_sec'], p['state']['joint_age_sec']) <= 0.15
                       for p in paired)
        coherent = coherent and current['state_valid'] and current['stationary'] and not current['busy']
        coherent = coherent and all(0 <= (self.node.get_clock().now().nanoseconds-v.get('association_stamp_ns', v['stamp_ns']))/1e9 <= 0.75
                                   for v in metadata['views'])
        recent = []
        for saved_path in sorted(self.journal.glob('*.json'), key=lambda p: p.stat().st_mtime, reverse=True)[:6]:
            saved = json.loads(saved_path.read_text())
            recent.append({'goal': saved['goal'], 'phase': saved['phase'],
                           'code': saved.get('result', {}).get('code', '')})
        metadata.update(robot_state=current, associated_states=paired, recent_attempts=recent,
                        observation_valid=coherent, previous_action_result=previous,
                        timings={'capture_wait_sec': captured.wait_sec,
                                 'capture_save_sec': captured.save_sec,
                                 'observe_total_sec': time.monotonic()-started})
        atomic_json(path, metadata)
        self.latest_observation = metadata if coherent else None
        return {'code': 'observed' if coherent else 'observation_unaligned',
                'metadata_path': str(path), **metadata}

    def handle(self, data):
        op = data.get('op', 'state')
        if op == 'state':
            return self.state()
        if op == 'query':
            return self.query_id(data['command_id'])
        if not self.operation_lock.acquire(blocking=False):
            return {'code': 'busy', 'message': 'One serial operation already in progress; query its command_id'}
        try:
            if op == 'observe':
                observation = self.observe(data.get('after_ns', 0))
                if data.get('roi') is not None and observation.get('observation_valid'):
                    import cv2
                    x, y, width, height = map(int, data['roi'])
                    image_path = Path(observation['metadata_path']).parent / 'rgb.png'
                    full = cv2.imread(str(image_path))
                    if min(x,y)<0 or min(width,height)<=0 or x+width>full.shape[1] or y+height>full.shape[0]:
                        raise ValueError('ROI outside full RGB image')
                    crop = image_path.with_name('rgb_detail.png')
                    if not cv2.imwrite(str(crop),full[y:y+height,x:x+width]):
                        raise OSError('ROI save failed')
                    observation['detail_view']={'file':str(crop),'roi':[x,y,width,height]}
                    atomic_json(Path(observation['metadata_path']), observation)
                return observation
            if op not in ('execute', 'check'):
                raise ValueError('unknown operation')
            goal_data = dict(data['goal'])
            command_id = goal_data.setdefault('command_id', str(uuid.uuid4()))
            # IDs are opaque to ROS; use a hash as the journal filename.
            import hashlib
            path = self.journal / (hashlib.sha256(command_id.encode()).hexdigest()+'.json')
            goal = self.goal(goal_data)
            if op == 'check':
                req = self.Check.Request()
                for field in ('kind', 'target', 'opening_m', 'force_n', 'timeout_sec'):
                    setattr(req, field, getattr(goal, field))
                req.check_ik = bool(data.get('check_ik', False))
                answer = self.convert(self.service(self.check, req, 8))
                answer['violations'] = workspace_violations(goal_data, answer['state'])
                return answer
            if path.exists():
                old = json.loads(path.read_text())
                if old['goal'] != goal_data:
                    return {'code': 'command_id_conflict', 'command_id': command_id}
                return {'command_id': command_id, 'cached': old, 'query': self.query_id(command_id),
                        'message': 'Duplicate request was not executed'}
            # Recover unresolved submissions before permitting any new ID.
            for saved_path in self.journal.glob('*.json'):
                saved = json.loads(saved_path.read_text())
                if saved['phase'] != 'finished':
                    previous_id = saved['goal']['command_id']
                    known = self.query_id(previous_id)
                    if not known['found'] or known['phase'] != 'finished':
                        return {'code': 'unresolved_previous_action', 'command_id': previous_id,
                                'query': known, 'message': 'No new motion until previous execution is resolved'}
                    saved.update(phase='finished', result=known)
                    atomic_json(saved_path, saved, durable=True)
            if self.latest_observation is None:
                return {'code': 'observation_required', 'message': 'Obtain and inspect a coherent observation before motion'}
            current = self.state()
            observed = self.latest_observation['robot_state']
            if not current['motion_allowed']:
                return {'code': 'not_ready', 'robot_state': current}
            # A subsequent external movement invalidates the planner's snapshot.
            a, b = current['flange_pose'], observed['flange_pose']
            displacement = sum((a['position'][axis]-b['position'][axis])**2 for axis in 'xyz')**0.5
            dot = abs(sum(a['orientation'][axis]*b['orientation'][axis] for axis in ('x','y','z','w')))
            if displacement>0.003 or dot<0.9998875 or abs(current['gripper_opening_m']-observed['gripper_opening_m'])>0.005:
                self.latest_observation = None
                return {'code': 'observation_required', 'message': 'Robot moved since the inspected observation'}
            if not self.action.wait_for_server(timeout_sec=2):
                raise RuntimeError('action_server_unavailable')
            # Persist BEFORE send: a crash in this window is conservatively unknown.
            record = {'goal': goal_data, 'phase': 'submission_pending', 'created_ns': time.time_ns()}
            atomic_json(path, record, durable=True)
            self.latest_observation = None
            started = time.monotonic()
            try:
                handle = self.wait(self.action.send_goal_async(goal), 5)
                if not handle.accepted:
                    result = self.query_id(command_id)
                else:
                    record['phase'] = 'accepted'
                    atomic_json(path, record, durable=True)
                    result = self.convert(self.wait(handle.get_result_async(), goal.timeout_sec+5).result)
                result['violations'] = workspace_violations(goal_data, result.get('state', {}))
                record.update(phase='finished', result=result)
                atomic_json(path, record, durable=True)
            except Exception as exc:
                record.update(phase='result_unknown', error=str(exc))
                atomic_json(path, record, durable=True)
                return {'command_id': command_id, **record,
                        'message': 'Query this ID; do not resubmit under a new ID until resolved'}
            after_ns = self.node.get_clock().now().nanoseconds
            # Wait for a publication after result; do not accept the old busy sample.
            try:
                deadline = time.monotonic()+1
                while time.monotonic()<deadline:
                    current = self.state()
                    if not current['busy'] and current['stationary'] and self.stamp_ns(current)>=after_ns:
                        break
                    time.sleep(0.02)
                observation = self.observe(after_ns, result)
            except Exception as exc:
                observation = {'code': 'observation_unavailable', 'message': str(exc)}
            record['observation'] = observation
            record['cycle_total_sec'] = time.monotonic()-started
            atomic_json(path, record, durable=True)
            return {'command_id': command_id, **record}
        finally:
            self.operation_lock.release()


def workspace_violations(goal, state):
    if goal.get('kind') not in ('move_to', 'move_linear'):
        return []
    bounds = state.get('workspace_bounds', [])
    position = goal.get('target', {}).get('pose', {}).get('position', {})
    return [{'axis': axis, 'requested': position.get(axis, 0),
             'allowed': bounds[2*i:2*i+2]}
            for i, axis in enumerate('xyz') if len(bounds)==6 and
            not bounds[2*i] <= position.get(axis, 0) <= bounds[2*i+1]]


def compact(value):
    if not isinstance(value, dict):
        return value
    result = {key: compact(item) if isinstance(item, dict) else item
              for key, item in value.items() if key != 'associated_states'}
    if 'metadata_path' in value and 'views' in value:
        result['images'] = [str(Path(value['metadata_path']).parent/view['file']) for view in value['views']]
        result['state_delta_sec'] = {item['role']: item['state_delta_sec'] for item in value.get('associated_states', [])}
        for key in ('views', 'previous_action_result', 'camera_device', 'schema_version'):
            result.pop(key, None)
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--socket', default=default_socket())
    sub = parser.add_subparsers(dest='mode', required=True)
    serve = sub.add_parser('serve')
    serve.add_argument('--journal', default='~/.local/state/astra_robot/actions')
    call = sub.add_parser('call')
    call.add_argument('request', help='JSON object or @file')
    args = parser.parse_args()
    if args.mode == 'call':
        raw = Path(args.request[1:]).read_text() if args.request.startswith('@') else args.request
        request = json.loads(raw)
        if request.get('op') == 'execute':
            request['goal'].setdefault('command_id', str(uuid.uuid4()))
        with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as connection:
            connection.settimeout(150)
            try:
                if request.get('op') == 'execute':
                    import sys
                    print('command_id='+request['goal']['command_id'], file=sys.stderr, flush=True)
                connection.connect(args.socket)
                connection.sendall(json.dumps(request).encode()+b'\n')
                answer = connection.makefile('rb').readline()
                if not answer:
                    raise ConnectionError('Bridge closed without returning a result')
            except (TimeoutError, OSError) as exc:
                print(json.dumps({'code': 'result_unknown', 'command_id': request.get('goal', {}).get('command_id'), 'error': str(exc)}))
                return
        print(answer.decode().strip())
        return
    os.umask(0o077)
    # Refuse an existing socket; never unlink another running bridge.
    class Handler(socketserver.StreamRequestHandler):
        def handle(self):
            try:
                raw = self.rfile.readline(1024*1024)
                request = json.loads(raw)
                answer = bridge.handle(request)
                if not request.get('verbose', False):answer = compact(answer)
            except Exception as exc:
                answer = {'code': 'client_error', 'message': str(exc)}
                if 'request' in locals():answer['command_id'] = request.get('goal', {}).get('command_id')
            try:
                self.wfile.write(json.dumps(answer, ensure_ascii=False).encode()+b'\n')
            except BrokenPipeError:
                pass  # Execute continues; its result remains in the journal.
    class Server(socketserver.ThreadingUnixStreamServer):
        daemon_threads = False
    # Bind before initializing ROS so a second daemon fails without side effects.
    with Server(args.socket, Handler) as server:
        bridge = None
        try:
            bridge = Bridge(args.journal)
            import signal
            def shutdown(signum, frame):
                threading.Thread(target=server.shutdown, daemon=True).start()
            signal.signal(signal.SIGTERM, shutdown)
            signal.signal(signal.SIGINT, shutdown)
            server.serve_forever()
        finally:
            # Let active requests finish before closing ROS.
            server.server_close()
            if bridge is not None:
                bridge.executor.shutdown()
                bridge.node.destroy_node()
                import rclpy
                rclpy.shutdown()
            Path(args.socket).unlink(missing_ok=True)


if __name__ == '__main__':
    main()
