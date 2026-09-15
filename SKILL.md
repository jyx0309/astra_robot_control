---
name: astra-robot-control
description: 使用三路图像和机器人状态，串行控制 CARM 机械臂并在目标拒绝后重新规划。
---

# CARM 机械臂操作

## 入口与闭环

首次使用按 [ROS 工作流](references/ros_workflow.md) 启动常驻客户端；它不会自动连接、使能或运动。硬件配置见 [hardware.md](references/hardware.md)。

1. 调用客户端 `observe`，读取三路图像、机器人状态、有效约束与近期动作结果。
2. 根据观察选择一个绝对目标，以新的 UUID 调用 `execute`。返回包含实际结果和动作后的新观察，查看图像后继续规划。
3. 需要预检查时调用 `check`；默认只查本地约束，`check_ik=true` 可请求 SDK 端点 IK。检查通过不证明路径无碰撞。

每次只执行一个动作；等待实际结果并重新观察后才决定下一目标。动作幅度由 Codex 根据场景判断。机械臂与所持工具的整个运动路径都应有空间。

## 接口约定

- 支持 `move_to`、`move_linear`、`set_gripper`。目标在 `base_link` 中，位置单位米，四元数顺序 `qx,qy,qz,qw`。
- 使用状态中的 `workspace_bounds` 等实际生效配置，不猜测限制。越界时调整目标。
- 历史字段 `flange_pose` 是 SDK Cartesian 反馈；查阅 `pose_reference`、`tool_index`。工具变换未核实时，不把它当成轴尖或已标定的物理法兰位置。
- `servo_enabled` 表示伺服反馈，`software_motion_enabled` 表示软件许可，`motion_allowed` 表示当前可接受新动作。兼容字段 `enabled=false` 本身不能证明硬件失能。
- `observation_valid` 必须为真；确认图像与状态时间关联。完整快照保存在返回的 `metadata_path`。

## 失败恢复

- `ik_rejected`、workspace 或目标参数拒绝：本次目标失败，重新观察并选新目标；不因此重启、重新使能或结束任务。
- 根据 `recent_attempts` 改变失败目标、姿态或接近路径。不按固定失败次数终止，也不原样反复提交失败目标。
- `result_unknown`、通信中断：用原 `command_id` 调用 `query`，先确认上一动作是否仍执行。不得换编号重复发送。相同编号只查询已有记录。
- 图像超时或时间不匹配：诊断缺失话题、时间戳并重试 `observe`，新观察可用后继续。
- 真实控制器/安全故障、明确碰撞、外部停止，或状态/观察不可用时暂停运动。恢复条件满足后再继续；无法安全推进时说明具体阻碍。

用户要求录像或决策记录时，读取 [记录流程](references/recording.md)。简短记录观察结论、下一动作和必要的不确定性即可。
