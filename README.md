# astra_robot_control

本目录包含用于 Insight9 手眼相机和 CARM 机械臂的 Codex 技能与 ROS 2 工作区。

## 系统概览

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

`robot_execution` 是确定性的安全边界，负责校验和执行每一个动作。

## 文件说明

- `SKILL.md`：闭环运行策略和安全边界。
- `scripts/`：状态查询、观察采集、单步执行和停止工具。
- `references/`：硬件预检、ROS 动作接口规范和手眼相机工作流。
- `ros_ws/src/insight_camera`：采集 RGB/左/右三路图像快照的 ROS 观察节点。
- `ros_ws/src/robot_execution`：CARM POSITION 模式执行器和安全校验。
- `ros_ws/src/robot_interfaces`：`ExecuteStep.action` 和机器人状态消息。

使用 ROS 2 Jazzy 在 `ros_ws/` 中构建；启动真实后端时，明确传入 CARM 硬件参数。

## 启动与执行

首次使用或源码发生变化后，在项目目录构建：

```bash
cd /path/to/astra_robot_control
source /opt/ros/jazzy/setup.bash
source ros_ws/vendor/arm_control_sdk/setup.bash
colcon build --base-paths ros_ws/src --symlink-install --build-base ros_ws/build --install-base ros_ws/install
source ros_ws/install/setup.bash
```

### 终端 1：启动 ROS bringup

默认使用 `mock` 后端，不会连接或使能真实机械臂：

```bash
cd /path/to/astra_robot_control
source /opt/ros/jazzy/setup.bash
source ros_ws/vendor/arm_control_sdk/setup.bash
source ros_ws/install/setup.bash
ros2 launch robot_bringup system.launch.py
```

真实硬件启动前，操作员必须确认工作区、急停、控制器、工具和夹爪配置。确认后，在终端 1 启动 CARM 后端：

```bash
cd /path/to/astra_robot_control
source /opt/ros/jazzy/setup.bash
source ros_ws/vendor/arm_control_sdk/setup.bash
source ros_ws/install/setup.bash
ros2 launch robot_bringup system.launch.py \
  backend:=carm \
  robot_ip:=10.42.0.101 \
  hardware_config_verified:=true \
  speed_level:=2.5
```

`speed_level` 的 SDK 范围是 `0~10`（约 `0%~100%`）。默认值为 `2.5`；如现场确认安全，可在启动时调高，最高为 `10`：

```bash
cd /path/to/astra_robot_control && \
source /opt/ros/jazzy/setup.bash && \
source ros_ws/vendor/arm_control_sdk/setup.bash && \
source ros_ws/install/setup.bash && \
ros2 launch robot_bringup system.launch.py \
  backend:=carm robot_ip:=10.42.0.101 \
  hardware_config_verified:=true speed_level:=10.0
```

`10.0` 是约 `100%` 速度，只适合在确认负载、工具、工作区和控制器配置后进行受控测试；普通抓取任务建议从 `2.5` 开始。

### 终端 2：检查并人工使能

`bringup` 只启动 ROS 节点，不会自动使能真实机械臂。打开第二个终端，加载同一个 ROS 工作区：

```bash
cd /path/to/astra_robot_control
source /opt/ros/jazzy/setup.bash
source ros_ws/install/setup.bash
./scripts/robot_status.sh
```

先确认上面 `robot_status.sh` 输出的连接、使能和故障状态，再执行使能命令。

确认工作区安全、机器人连接正常且无故障后，在同一终端执行：

```bash
ros2 service call /robot/enable std_srvs/srv/Trigger "{}"
```

### Codex 会话：提交任务目标

完成使能后，在项目目录打开 Codex 会话，并直接描述任务目标，例如：

```text
请抓取桌面上的红色手机，并把它抬升到安全高度。
```

或：

```text
请把相机视野中左侧的方形物体抓起来，放到右侧空白区域。
```

Codex 会使用本技能自动执行以下闭环，不需要用户手动逐条输入观察和动作命令：

```text
读取机器人状态和三路图像
→ 选择一个受限的绝对末端目标
→ 执行一个动作
→ 等待动作结果
→ 重新读取状态和图像
→ 决定下一步
```

手动观察和动作命令只用于调试或验证接口：

```bash
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
