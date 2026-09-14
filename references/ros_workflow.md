# ROS 工作流

```bash
cd /path/to/astra_robot_control/ros_ws
source /opt/ros/jazzy/setup.bash
source vendor/arm_control_sdk/setup.bash
source install/setup.bash
```

## MVP 闭环

每次迭代始终为：

```text
robot_status.sh
→ capture_observation.sh
→ Codex 运行时读取 RGB/左目/右目图像和当前 `/robot/state`
→ Codex 选择下一个绝对的 `base_link` 笛卡尔目标位姿/动作
→ 审批器校验、限幅或拒绝该动作
→ `/robot/execute_step`
→ 等待结果
→ 再次采集
```

在获知第一个动作结果前，Codex 智能体不得发出第二个动作。抓取任务通常包含多次接近修正、一次 `set_gripper`，然后进行小幅抬升。

`ExecuteStep.action` 仍是线协议。目前支持 `move_to`、`move_linear` 和 `set_gripper`。Codex 直接提供下一个绝对的 `base_link` 位姿；正常闭环不包含固定距离或相对工具坐标系适配器。执行器负责校验、工作空间检查、超时处理，以及 CARM SDK 的 IK/轨迹执行。

## 启动与检查

```bash
ros2 run robot_execution execution_node --ros-args \
  -p backend:=carm -p robot_ip:=10.42.0.101 \
  -p hardware_config_verified:=true
ros2 service call /robot/enable std_srvs/srv/Trigger "{}"
ros2 topic echo /robot/state --once
```

## 相机观察

```bash
ros2 service call /insight_observer/capture std_srvs/srv/Trigger "{}"
```

返回的 `metadata.json` 会引用 `rgb.png`、`left.png` 和 `right.png`。三路视觉输入都会传给 Codex；深度不属于当前 MVP 的观察或决策链，待后续标定完成后再作为独立增强功能加入。

## 单步动作

```bash
ros2 action send_goal /robot/execute_step robot_interfaces/action/ExecuteStep \
  "{command_id: manual-1, kind: set_gripper, opening_m: 0.03, force_n: 2.0, timeout_sec: 5.0}" --feedback
```

Pose values are `[x,y,z,qx,qy,qz,qw]` in `base_link`, meters and quaternion `xyzw`.

## 闭环停止规则

Codex 根据当前观察和机器人状态决定每个绝对目标位姿。不要在技能中编码固定的 5/10/20 mm 行为或相对距离转换。每次动作后都要重新观察。仅缺少度量深度不是停止条件；只有出现执行/安全硬故障、明确碰撞证据、目标反复丢失或没有视觉响应、紧急/外部停止，或达到配置的最大动作数时才停止。


## 设计边界

模型负责选择下一个笛卡尔目标位姿和动作幅度。技能不会将行为量化为固定平移档位。本地审批器/`robot_execution` 层负责 CARM 专用有效性检查、工作空间和禁入区域限制、速度/超时限制、四元数检查、IK 可达性响应以及限幅/拒绝行为。这些是本地硬件适配，不是对公开 Astra 实现的声明。
