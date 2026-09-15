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

## 记录推理画面和文字

`scripts/record_inference.py` 使用 Python 3 标准库，保存每次送给模型的观察图像、显式决策摘要、可选状态及动作结果，并生成本地 HTML 回放。它不会自动抓取聊天或模型内部隐藏思维，也不是相机连续视频录像；每一步文字由调用方明确写入。记录程序不会控制机械臂。

在本 README 所在目录创建会话：

```bash
python3 scripts/record_inference.py init recordings/demo --task '抓取目标物体'
```

每次采集并查看图像后，使用相机返回的实际元数据路径记录本轮决策：

```bash
python3 scripts/record_inference.py add recordings/demo \
  --observation /实际观察目录/metadata.json \
  --summary '目标位于画面中央，下一步调整夹爪对准目标。'
```

命令输出唯一步骤编号。长文本可用 `--summary-file decision.txt` 传入；`--state-file state.txt` 和 `--action-file action.txt` 可附加当轮真实状态及计划动作。文件均按 UTF-8 保存原文。

动作完成后，将实际返回结果写入文件，再关联到刚才的步骤：

```bash
python3 scripts/record_inference.py result recordings/demo 实际步骤编号 \
  --result-file action_result.txt
```

直接在浏览器打开 `recordings/demo/index.html` 即可查看三路图像、决策摘要、状态及结果。每次写入自动更新页面，刷新可看新增步骤；`export recordings/demo` 可重建页面。缺失的状态和结果显示“未记录”。

每步保存独立 `record.json`、图片副本和原始 `source_metadata.json`，整个会话目录可复制回放。相机时间保留为原始 `stamp_ns`，文字及结果记录时间使用 UTC，并非模型推理起止时间。归档图片路径以 `record.json` 为准，原始元数据保持不变。

后续可直接对 Codex 说：“执行任务，并把每次观察、决策摘要和动作结果记录到 recordings/本次任务。”目前已完成离线验证，尚未验证真实机械臂上的记录流程。

### 直接导出 MP4

```bash
python3 scripts/record_inference.py video recordings/demo
```

输出为会话目录内的 `replay.mp4`：1280×720、10 fps、三路图像并排显示，中文决策摘要直接绘制到画面，无音轨。默认每步停留 3 秒；长摘要自动分页，每页 3 秒，不会截断。可用 `--seconds 5` 调整每页时长。视频是观察帧回放，不代表真实动作耗时；状态和完整动作结果仍保留在网页与 JSON 中。

视频导出需要 Pillow、numpy、OpenCV 和中文字体；当前环境已具备。默认字体为 `/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc`，可通过 `--font` 指定。采用 MP4V 编码，需要支持此编码的播放器。重新导出会原子替换当前会话的 `replay.mp4`；失败时保留原视频。

原始相机图片当前保存在 `/home/yuxuan/insight_captures/<观察编号>/`。使用上面的相对路径示例且在本技能目录执行时，归档位于 `/home/yuxuan/astra_robot_control/skill/recordings/demo/`；其中 `steps/` 保存图像与文字，`index.html` 为网页回放，`replay.mp4` 为视频。记录目录已加入 Git 忽略列表。

## 安全边界

- 默认 `backend` 是 `mock`；真实硬件必须显式传入 `backend:=carm`。
- 机械臂使能、首次运动和急停由操作员掌握；Codex 负责使能后的受限逐步闭环。
- 每次动作完成后必须重新获取状态和三路图像，不能预先连续发送多个动作。
- `robot_execution` 负责工作空间、姿态、IK、超时和故障校验；不设置固定平移/旋转步长。
- 已确认的 IK 拒绝返回 `ik_rejected`，重新观察和规划；不急停或重新使能。SDK 返回值不明时继续监测实际执行，通信超时须查询原动作，不能盲目重发。真实故障、运动超时或取消仍执行停止流程。

常驻客户端、失败恢复和最新接口见 [ROS 工作流](references/ros_workflow.md)。常规循环优先使用该入口，避免每轮启动多个 ROS CLI。
