#!/usr/bin/env python3
"""
carm_demo.py —— 六轴单臂 SDK 示例（基于 CArmSingleCol）。

演示 CArmSingleCol 的常用接口用法，包括连接、基础控制、状态查询、
运动指令、夹爪/末端执行器、运动学、示教、回调注册等。
"""

import time
from typing import List

from carm import CArmSingleCol

# ==================== 全局实例 ====================
carm_: CArmSingleCol = None


# ==================== 基础操作 ====================

def connect(ip: str = "10.42.0.101", port: int = 8090, timeout: float = 1.0) -> int:
    ret = carm_.connect(ip, port, timeout)
    print(f"connect, ret = {ret}")
    return ret

def disconnect() -> int:
    ret = carm_.disconnect()
    print(f"disconnect, ret = {ret}")
    return ret

def is_connect() -> bool:
    return carm_.is_connected()

def set_ready() -> int:
    ret = carm_.set_ready()
    print(f"set_ready, ret = {ret}")
    return ret

def set_servo_enable(flag: bool) -> int:
    ret = carm_.set_servo_enable(flag)
    print(f"set_servo_enable, ret = {ret}")
    return ret

def set_control_mode(mode: int) -> int:
    ret = carm_.set_control_mode(mode)
    print(f"set_control_mode, ret = {ret}")
    return ret

def emergency_stop() -> int:
    ret = carm_.emergency_stop()
    print("emergency_stop called.")
    return ret

def get_version() -> str:
    version = carm_.get_version()
    print(f"get_version, version = {version}")
    return version

def get_config():
    config = carm_.get_config()
    print("Joint limits params: ", config)
    return config

def get_arm_status():
    status = carm_.get_status()
    print("\n=== 机械臂运行状态信息 ===")
    print(f"机械臂编号: {status.get('arm_index')}")
    print(f"机械臂名称: {status.get('arm_name')}")
    print(f"机械臂自由度: {status.get('arm_dof')}")
    print(f"伺服状态: {status.get('servo_status')}")
    print(f"控制器状态: {status.get('state')}")
    print(f"FSM模式: {status.get('fsm_state')}")
    print(f"速度标幺值: {status.get('speed_percentage')}")
    print(f"是否仿真模式: {status.get('on_debug_mode')}")
    print("==========================\n")

# ==================== 状态获取 ====================

def get_joint_pos() -> List[float]:
    pos = carm_.get_joint_pos()
    print(f"get_joint_pos, pos = {pos}")
    return pos

def get_joint_vel() -> List[float]:
    vel = carm_.get_joint_vel()
    print(f"get_joint_vel, vel = {vel}")
    return vel

def get_joint_tau() -> List[float]:
    tau = carm_.get_joint_tau()
    print(f"get_joint_tau, tau = {tau}")
    return tau

def get_plan_joint_pos() -> List[float]:
    pos = carm_.get_plan_joint_pos()
    print(f"get_plan_joint_pos, pos = {pos}")
    return pos

def get_plan_joint_vel() -> List[float]:
    vel = carm_.get_plan_joint_vel()
    print(f"get_plan_joint_vel, vel = {vel}")
    return vel

def get_plan_joint_tau() -> List[float]:
    tau = carm_.get_plan_joint_tau()
    print(f"get_plan_joint_tau, tau = {tau}")
    return tau

def get_cart_pose() -> List[float]:
    pose = carm_.get_cart_pose()
    print(f"get_cart_pose, pose(x,y,z,Qx,Qy,Qz,Qw) = {pose}")
    return pose

def get_plan_cart_pose() -> List[float]:
    pose = carm_.get_plan_cart_pose()
    print(f"get_plan_cart_pose, pose(x,y,z,Qx,Qy,Qz,Qw) = {pose}")
    return pose

def get_joint_external_tau() -> List[float]:
    tau = carm_.get_joint_external_tau()
    print(f"get_joint_external_tau, tau = {tau}")
    return tau

def get_cart_external_force() -> List[float]:
    force = carm_.get_cart_external_force()
    print(f"get_cart_external_force, force(fx,fy,fz,tx,ty,tz) = {force}")
    return force

# ==================== 末端执行器（通用 eeff） ====================

def get_eeff_state() -> int:
    state = carm_.get_eeff_state()
    print(f"get_eeff_state, state = {state}")
    return state

def get_eeff_pos() -> List[float]:
    pos = carm_.get_eeff_pos()
    print(f"get_eeff_pos, pos = {pos}")
    return pos

def get_eeff_vel() -> List[float]:
    vel = carm_.get_eeff_vel()
    print(f"get_eeff_vel, vel = {vel}")
    return vel

def get_eeff_tau() -> List[float]:
    tau = carm_.get_eeff_tau()
    print(f"get_eeff_tau, tau = {tau}")
    return tau

def set_eeff(pos: list, vel: list, tau: list) -> int:
    ret = carm_.set_eeff(pos, vel, tau)
    print(f"set_eeff, ret = {ret}")
    return ret

# ==================== 运动控制 ====================

def track_joint(targets: List[float], eeff_pos: float = -1.0) -> int:
    ret = carm_.track_joint(targets, eeff_pos)
    print(f"track_joint, ret = {ret}")
    return ret

def track_pose(targets: List[float], eeff_pos: float = -1.0) -> int:
    ret = carm_.track_pose(targets, eeff_pos)
    print(f"track_pose, ret = {ret}")
    return ret

def move_joint(targets: List[float], desire_time: float = -1.0, is_sync: bool = True) -> int:
    ret = carm_.move_joint(targets, desire_time, is_sync)
    print(f"move_joint, ret = {ret}")
    return ret

def move_pose(targets: List[float], desire_time: float = -1.0, is_sync: bool = True) -> int:
    ret = carm_.move_pose(targets, desire_time, is_sync)
    print(f"move_pose, ret = {ret}")
    return ret

def move_line_joint(targets: List[float], is_sync: bool = True) -> int:
    ret = carm_.move_line_joint(targets, is_sync)
    print(f"move_line_joint, ret = {ret}")
    return ret

def move_line_pose(targets: List[float], is_sync: bool = True) -> int:
    ret = carm_.move_line_pose(targets, is_sync)
    print(f"move_line_pose, ret = {ret}")
    return ret

def move_joint_traj(target_pos: List[List[float]], eeff_pos: list = None,
                    stamps: list = None, is_sync: bool = True) -> int:
    ret = carm_.move_joint_traj(target_pos, eeff_pos, stamps, is_sync)
    print(f"move_joint_traj, ret = {ret}")
    return ret

def move_pose_traj(target_pos: List[List[float]], eeff_pos: list = None,
                   stamps: list = None, is_sync: bool = True) -> int:
    ret = carm_.move_pose_traj(target_pos, eeff_pos, stamps, is_sync)
    print(f"move_pose_traj, ret = {ret}")
    return ret

def move_flow_pose(target_pos: List[float], line_theta_weight: float = 0.5,
                   accuracy: float = 0.0001, is_sync: bool = True) -> int:
    ret = carm_.move_flow_pose(target_pos, line_theta_weight, accuracy, is_sync)
    print(f"move_flow_pose, ret = {ret}")
    return ret

# ==================== 设置与配置 ====================

def set_speed_level(level: float, response_level: int = 20) -> int:
    ret = carm_.set_speed_level(level, response_level)
    print(f"set_speed_level, ret = {ret}")
    return ret

def set_tool_index(index: int) -> int:
    ret = carm_.set_tool_index(index)
    print(f"set_tool_index, ret = {ret}")
    return ret

def get_tool_index() -> int:
    return carm_.get_tool_index()

def get_tool_coordinate(index: int) -> list:
    return carm_.get_tool_coordinate(index)

def set_collision_config(enable_flag: bool = True, sensitivity_level: int = 0) -> int:
    ret = carm_.set_collision_config(enable_flag, sensitivity_level)
    print(f"set_collision_config, ret = {ret}")
    return ret

# ==================== 示教相关 ====================

def trajectory_teach(off_on: bool, name: str) -> int:
    ret = carm_.trajectory_teach(off_on, name)
    print(f"trajectory_teach, ret = {ret}")
    return ret

def trajectory_recorder(name: str, is_sync: bool = True) -> int:
    ret = carm_.trajectory_recorder(name, is_sync)
    print(f"trajectory_recorder, ret = {ret}")
    return ret

def check_teach() -> list:
    traj_list: list = []
    carm_.check_teach(traj_list)
    print(f"check_teach, traj_list = {traj_list}")
    return traj_list

# ==================== 运动学 ====================

def inverse_kine(tool_index: int, quat_pose: List[float],
                 ref_joint: List[float]) -> List[float]:
    jnt_value: List[float] = []
    ret = carm_.inverse_kine(tool_index, quat_pose, ref_joint, jnt_value)
    print(f"inverse_kine, ret = {ret}, jnt_value = {jnt_value}")
    return jnt_value

def forward_kine(tool_index: int, jnt_value: List[float]) -> List[float]:
    quat_pose: List[float] = []
    ret = carm_.forward_kine(tool_index, jnt_value, quat_pose)
    print(f"forward_kine, ret = {ret}, quat_pose = {quat_pose}")
    return quat_pose

# ==================== 回调 ====================

def joint_publisher(t: float, p: List[float], v: List[float], a: List[float]):
    if carm_.get_eeff_state() >= 0:
        eeff_pos = carm_.get_eeff_pos()
        eeff_vel = carm_.get_eeff_vel()
        eeff_tau = carm_.get_eeff_tau()
        print(f"time: {t}")
        print(f"joint_pos: {p}, eeff_pos: {eeff_pos}")
        print(f"joint_vel: {v}, eeff_vel: {eeff_vel}")
        print(f"joint_tau: {a}, eeff_tau: {eeff_tau}")
    else:
        print(f"time: {t}, joint_pos: {p}")

def pose_publisher(t: float, p: List[float]):
    print(f"time: {t}, cart_pose(xyz-xyzw): {p}")

def on_carm_error(code: int, msg: str):
    if code == 3001:
        print(f"receive a warning, msg = {msg}")
    else:
        print(f"receive an error, code = {code}, msg = {msg}")

def on_task_completion(task_key: str):
    print(f"task_completion: {task_key}")


# ==================== main ====================

if __name__ == '__main__':
    print("Init connection...")
    carm_ = CArmSingleCol("10.42.0.101")
    time.sleep(1)

    print("注册回调")
    carm_.register_error_cbk("err", on_carm_error)
    carm_.register_completion_cbk("done", on_task_completion)
    carm_.register_joint_cbk("joint", joint_publisher)
    carm_.register_pose_cbk("pose", pose_publisher)

    print("链接完毕，使能机械臂")
    set_ready()
    time.sleep(1)

    joint_ = [0, 0, 0, 0, 0, 0]
    print(f"移动到安全位置: {joint_}")
    carm_.move_joint(joint_, is_sync=True)

    # 1. 获取机器人的基础配置与状态
    get_version()
    get_config()
    get_arm_status()
    print("当前关节位置: ", get_joint_pos())
    print("当前末端位姿: ", get_cart_pose())

    # 2. 运动控制测试
    target_pose = get_cart_pose()
    if target_pose and len(target_pose) >= 7:
        target_pose[2] += 0.05  # Z轴向上移动5cm
        print(f"运动到新位姿: {target_pose}")
        move_pose(target_pose, is_sync=True)

    # 测试末端执行器
    print("设置末端执行器位置...")
    set_eeff([0.04], [], [15.0])
    time.sleep(2)

    # 测试末端力与速度信息获取
    print("末端力矩: ", get_cart_external_force())

    # 3. 恢复到安全位置
    print("任务执行完毕，恢复到安全位置")
    carm_.move_joint(joint_, is_sync=True)

    time.sleep(1)

    # 释放回调
    carm_.release_joint_cbk("joint")
    carm_.release_pose_cbk("pose")
    carm_.release_error_cbk("err")
    carm_.release_completion_cbk("done")

    print("断开连接")
    disconnect()

