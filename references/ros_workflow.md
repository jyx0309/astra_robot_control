# ROS 工作流

## 构建与启动

在 `skill/ros_ws` 中：

```bash
source /opt/ros/jazzy/setup.bash
colcon build --packages-select robot_interfaces robot_execution insight_camera robot_bringup
source install/setup.bash
```

本次修改扩展了 ROS 消息/服务；需将使用这些接口的节点一起更新，不能混用旧接口。构建不会自动重启任何正在运行的实机节点。

执行器仍须按硬件配置显式启动、连接和使能；常驻客户端不承担这些动作。机器人已经就绪时，在 `skill` 中运行：

```bash
source /opt/ros/jazzy/setup.bash
source ros_ws/install/setup.bash
python3 scripts/robot_client.py serve
```

另一个终端用轻量客户端发请求（`call` 自身不导入 ROS）：

```bash
python3 scripts/robot_client.py call '{"op":"state"}'
python3 scripts/robot_client.py call '{"op":"observe"}'
python3 scripts/robot_client.py call @/absolute/path/request.json
```

`request.json` 示例（检查目标，不运动）：

```json
{
  "op": "check",
  "check_ik": false,
  "goal": {
    "kind": "move_to",
    "timeout_sec": 30.0,
    "target": {
      "header": {"frame_id": "base_link"},
      "pose": {
        "position": {"x": 0.3, "y": 0.0, "z": 0.3},
        "orientation": {"x": 0.0, "y": 0.0, "z": 0.0, "w": 1.0}
      }
    }
  }
}
```

实际目标由当前观察决定，不能照抄示例执行。`op=execute` 时 `goal.command_id` 应使用 UUID；CLI 缺省会生成。动作参数与 ROS `ExecuteStep.Goal` 相同。客户端等待动作结果，再获取新观察；完整状态及时间关联写入 `metadata_path`，常规返回减少重复状态。`verbose=true` 返回全部字段。

`observe` 可传 `roi: [x,y,width,height]` 输出已旋转 RGB 图中的局部细节；保留全部原图，裁剪不会自动推断目标。

## 结果与恢复

- `/robot/check_step`：返回有效性、原因、实际约束和检查耗时；可选 SDK 端点 IK，`path_checked=false`。
- `/robot/query_step`：按 `command_id` 查询本节点生命周期内的结果。动作接收和终态都可查询，重复 ID 不会再次执行。
- 常驻客户端将提交意图与结果持久化到 `~/.local/state/astra_robot/actions`，在发送前落盘。客户端重启后也不会盲目重复提交。
- 请求失败可能表示响应丢失。用 `{"op":"query","command_id":"原编号"}` 核对；历史结果与当前状态分开表达。
- 执行器重启后不能恢复 SDK 内部队列；未知且未完成的记录阻止新提交。须核实控制器队列与实际状态后再人工处理该记录，不能以删除日志代替核实。
- IK 回调缺少任务 ID，无法严格证明延迟事件属于哪个动作。实现以接收序号隔离旧事件，等待新鲜静止样本后释放动作槽；不会自动重放动作。重复通知不变成持久硬故障，真实故障始终优先。
- `ik_rejected` 只匹配已确认的 `3001:[17] unable to cal ik`；其他错误不统一忽略。
- 无固定动作/失败次数上限；单动作超时用于识别未结束的运动。超时仍请求停止，之后必须核对实际状态。

## 观察与性能

`/insight_observer/capture_after` 接收 ROS 时间边界，只选边界之后且新鲜的三路帧。显式配置 `timestamp_mode`：`ros` 使用同域源时间；当前 Insight9 使用 `device`，保留设备运行时间并根据持续到达数据估算主机关联时间。三路源时间仍须相互同域。重复帧被丢弃、时间回退重新预热、相对积压按已建立的偏移检查。`association_stamp_ns` 用于状态关联，`stamp_ns` 始终保存原始值；恒定传输延迟无法由此确定，估算不等于硬件曝光同步。

快照包含图像时间差、关联状态、状态样本年龄、近期尝试和耗时。`flange_pose` 的字段名为兼容保留，参考点标记为 `sdk_cartesian_active_tool_unverified`；未标定的工具偏移不被假定为零。

耗时字段：`sdk_refresh_ms`、`sdk_submit_sec`、`total_sec`、`capture_wait_sec`、`capture_save_sec`、`observe_total_sec`、`cycle_total_sec`。高频反馈降至约 2 Hz，CLI 默认不打印完整反馈；相机 PNG 使用低压缩保存。SDK 调用仍保持串行，耗时超过 100 ms 的状态刷新产生诊断；在确认 SDK 线程安全和实机测量前不并发调用 SDK。

`robot_status.sh --diagnostics` 做完整 ROS 图检查；普通状态检查不再每轮列所有节点、服务和动作。旧脚本保留为诊断兼容入口，常规闭环使用常驻客户端。

工作空间目前只检查目标参考点，SDK 碰撞配置保持原控制器设置。端点 IK 与空间合法性均不证明整条轨迹或工具包络安全；工具标定、禁入区域和路径规划须依据真实几何另行配置，不能编造场景边界。

控制器 `state=1` 可以表示 POSITION 模式下保持位置；不能仅凭它判断正在运动。静止判定使用至少 250 ms 的关节位置窗口及速度均值，同时拒绝缺失/过期反馈、明显运动和硬故障。单帧速度噪声不直接等同于运动。

## 离线回归

```bash
# 在 skill/ros_ws 中，已 source 上述环境
colcon test --packages-select robot_execution insight_camera
colcon test-result --verbose
ROS_DOMAIN_ID=187 python3 -m pytest -q ../tests/test_recovery_integration.py
```

集成测试显式启动 `backend=mock` 和合成三路相机；不连接或使能实机。真实控制器的 IK、工具语义、路径保护和硬件耗时仍需实机验证。
