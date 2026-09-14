# astra_robot_control

本目录包含用于 Insight9 手眼相机和 CARM 机械臂的 Codex 技能与 ROS 2 工作区。

## MVP 架构

首个版本复现 Astra 风格的闭环流程：

```text
wrist RGB 图像 + 末端位姿 + 关节状态 + 夹爪状态
    ↓
Codex 运行时读取观察结果并决定下一个笛卡尔目标位姿/动作
    ↓
审批器和 robot_execution 负责校验、限幅或拒绝
    ↓
ROS 2 ExecuteStep.action
    ↓
robot_execution 校验并调用 CARM SDK 执行
    ↓
新的腕部观察结果
```

模型不需要在每一步之前重建手机在 `base_link` 中的精确位置。它应通过模型反复选择的动作完成接近、对齐、闭合和抬升。`robot_execution` 始终是确定性的安全边界。

## 文件说明

- `SKILL.md`：闭环运行策略和安全边界。
- `scripts/`：状态查询、观察采集、单步执行和停止工具。
- `references/`：硬件预检、ROS 动作接口规范和手眼相机工作流。
- `ros_ws/src/insight_camera`：采集 RGB/左/右三路图像快照的 ROS 观察节点。
- `ros_ws/src/robot_execution`：CARM POSITION 模式执行器和安全校验。
- `ros_ws/src/robot_interfaces`：`ExecuteStep.action` 和机器人状态消息。

## 工作阶段

1. **MVP**：RGB/左右单色图像加本体感知；每次观察执行一个受限动作，重复直到视觉上完成抓取/抬升。
2. **深度增强**：后续再加入 SDK 深度回调、标定、相机到工具的 TF，以及度量 3D 比较。
3. **硬件标定**：相机到工具的测量、工具/基座 TF 校验、禁入区域调节和抓取评估。

使用 ROS 2 Jazzy 在 `ros_ws/` 中构建；启动真实后端时，明确传入 CARM 硬件参数。

## 启动与执行

首次使用或源码发生变化后，在项目目录构建：

```bash
cd /path/to/astra_robot_control/ros_ws
source /opt/ros/jazzy/setup.bash
source vendor/arm_control_sdk/setup.bash
colcon build --base-paths src --symlink-install
source install/setup.bash
```

推荐使用一个 ROS 终端启动完整系统。默认使用 `mock` 后端，不会连接或使能真实机械臂：

```bash
cd /path/to/astra_robot_control/ros_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 launch robot_bringup system.launch.py
```

真实硬件启动前，操作员必须确认工作区、急停、控制器、工具和夹爪配置。确认后启动 CARM 后端：

```bash
source vendor/arm_control_sdk/setup.bash
ros2 launch robot_bringup system.launch.py \
  backend:=carm \
  robot_ip:=10.42.0.101 \
  hardware_config_verified:=true
```

启动 bringup 不等于使能机械臂。操作员确认安全后，单独执行：

```bash
ros2 service call /robot/enable std_srvs/srv/Trigger "{}"
```

建议在另一个终端打开 Codex 任务，由 Codex 使用本技能执行闭环；不需要再次启动 bringup。启动后可用以下命令检查状态和获取观察：

```bash
./scripts/robot_status.sh
./scripts/capture_observation.sh
```

单步动作必须等待结果并重新观察后才能执行下一步。例如夹爪动作：

```bash
./scripts/execute_step.sh set_gripper 0.03 2.0
```

移动动作的目标位姿为 `base_link` 下的绝对位姿，格式为 `[x y z qx qy qz qw]`：

```bash
./scripts/execute_step.sh move_to X Y Z QX QY QZ QW
```

任何时候需要停止时执行：

```bash
./scripts/stop_robot.sh
```

## 安全边界

- 默认 `backend` 是 `mock`；真实硬件必须显式传入 `backend:=carm`。
- 机械臂使能、首次运动和急停由操作员掌握；Codex 负责使能后的受限逐步闭环。
- 每次动作完成后必须重新获取状态和三路图像，不能预先连续发送多个动作。
- `robot_execution` 负责工作空间、步长、姿态、IK、超时和故障校验。

## 自主闭环策略

Codex 运行时会在一个任务中执行有限轮次的观察/动作循环：每轮采集腕部 RGB、左红外和右红外图像，读取当前本体感知信息，选择一个绝对的 `base_link` 目标位姿，执行后再次采集。轮次之间不需要用户消息。该技能不规定固定的 5/10/20 mm 行为，也不把所选距离转换成位姿；审批器和 robot_execution 负责执行硬限制，并可能进行限幅或拒绝。
只有发生机器人/安全硬故障、明确的碰撞证据、目标反复丢失或动作无效、外部停止，或达到最大轮数时才终止。该循环在 Codex 任务结束时结束，不是后台规划进程。

## 公共设计与本地适配的边界

这里复现的公开 Astra 风格设计是串行闭环：模型接收腕部 RGB 和本体感知信息，提出下一个笛卡尔目标/动作；安全/审批层进行约束；执行一个 ROS 动作；在提出下一次建议前采集新的观察结果。

本配置所需的本地适配包括 `ExecuteStep.action` ROS 线协议、CARM SDK POSITION 模式执行和内部 IK、Insight9 话题处理、当前机器人状态新鲜度检查，以及工作空间/碰撞/超时限制。这些工程安全措施和硬件接口并不表示它们是公开 Astra 项目的固定参数。
