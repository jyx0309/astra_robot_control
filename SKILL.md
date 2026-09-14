---
name: astra-robot-control
description: 通过安全的串行闭环，使用多视角图像和机器人本体状态控制 CARM 机械臂。
---

# Astra 机械臂控制

本 Skill 复现纯视觉与 proprioception 驱动的 Astra-style 闭环。Codex 是唯一的高层视觉 planner；ROS 内不启动第二个 GPT/VLM planner。

核心闭环严格串行：

```text
images + current proprioception
  → Codex 决定一个绝对 Cartesian EE target pose
  → safety/approver
  → ExecuteStep
  → robot_execution
  → new observation
  → Codex replans
```

最小 observation 结构：

```json
{
  "images": ["rgb.png", "left.png", "right.png"],
  "eef_pose_base": ["x", "y", "z", "qx", "qy", "qz", "qw"],
  "joint_positions": [],
  "gripper_state": {"opening_m": 0.0, "force_n": 0.0},
  "robot_status": {"connected": true, "enabled": true, "fault": ""},
  "previous_action_result": {}
}
```

接口语义：

1. `eef_pose_base` 是 `base_link` 下当前绝对末端位姿，位置单位为米。
2. `move_to` 的输入是 `base_link` 下的绝对目标末端位姿。
3. orientation 使用 quaternion，顺序为 `qx, qy, qz, qw`。
4. 每轮只能执行一个动作。
5. 每次动作完成后必须重新获取 observation。
6. 不允许提前规划并连续执行多步。
7. safety、workspace、step limits、IK 和 fault handling 全部由 `robot_execution` 负责。
8. MVP 不使用自定义 IK、MoveIt 或显式 RGB-D 几何链。
9. RGB-only 带来的空间不确定性不能单独作为停止理由。

执行规则：

1. 运行 `scripts/robot_status.sh`，确认机器人状态最新、已使能且无故障。
2. 运行 `scripts/capture_observation.sh`，获取三路图像；将其与当前绝对 EE pose、本体状态及上一动作结果交给 Codex。
3. Codex 直接选择一个绝对 `base_link` 目标位姿，不能先选择固定距离或套用手写视觉伺服规则。
4. 通过 `scripts/execute_step.sh` 或 ROS `ExecuteStep.action` 执行一个动作。
5. 等待实际动作结果，再获取新的三路图像和机器人状态，然后重新规划。

停止条件仅包括：机器人/控制器或安全故障、非法状态、workspace/step limit 或 IK 拒绝、明确碰撞证据、目标持续丢失、动作后持续没有预期视觉变化、达到动作上限或外部停止。仅缺少深度、TF 或 RGB-D 几何信息不是停止条件。

MVP 不包含相机轴到图像方向的人工映射、camera-to-tool TF、pixel-to-3D、RGB-depth 对齐、固定远中近步长或 relative visual servo controller。相关内容如需加入，应作为后续独立 enhancement，不得改变本 Skill 的最小闭环接口。
