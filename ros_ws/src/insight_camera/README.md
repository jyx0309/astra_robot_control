# Insight9 ROS 相机观察包

本包消费相机原生 ROS 2 数据，不需要编译 insight-sdk，也不调用机械臂或 GPT。

## 构建和启动（Ubuntu 24.04 / ROS 2 Jazzy）

```bash
cd /home/yuxuan/astra_robot_control/skill/ros_ws
source /opt/ros/jazzy/setup.bash
colcon build --base-paths src --packages-select insight_camera --symlink-install
source install/setup.bash
ros2 launch insight_camera camera.launch.py
```

相机切到 ROS 模式，确认设备与主机 ROS_DOMAIN_ID 一致。DDS 按设备配置；厂商建议 Fast DDS 作为初次连接基线。启用设备与主机时间同步，时间戳必须与节点 ROS 时钟同域。本包拒绝请求前采集或时间戳在未来的帧。

MVP 只要求左目、右目和 RGB 三路输入，按整组图像的最大时间差不超过 sync_tolerance 匹配。RGB 与左右红外图像进入 Codex 视觉输入，由 Codex 比较多视角变化并结合机器人本体状态判断下一步绝对末端位姿；深度暂不参与主决策链。

```bash
ros2 topic list -t
ros2 service call /insight_observer/capture std_srvs/srv/Trigger '{}'
```

成功返回 success: true 和 metadata.json 的绝对路径；失败返回原因。每次请求在服务器开始处理时建立新帧边界，等待最多 capture_timeout 秒。调用方应在机械臂动作完成后发起请求。服务顺序处理，相机订阅由其他执行线程持续接收。

快照默认保存到 ~/insight_captures/<唯一编号>/，包括 left.png、right.png、rgb.png 和 metadata.json。所有必需输入的最大与最小时间戳之差不超过 sync_tolerance（默认 80ms），这不是硬件同步保证。

当前 MVP 不保存或使用深度、CameraInfo 和相机 TF。后续深度增强功能必须单独加入，并要求有效外参和明确深度单位。RGB-only 模式不要求标定。

## 参数

- rgb_topic：RGB 设备话题。
- rgb_transport：compressed（默认）或 raw；raw 支持 cv_bridge 标准彩色编码及 NV12。
- left_topic / right_topic：默认 infra1 / infra2 的 image_rect_raw。
- require_stereo：默认 true，缺失任一路时超时失败，不降级为单图。
- capture_timeout：等待新观察的秒数；sync_tolerance：三路图像的最大时间差。
- output_dir：快照目录，支持 ~。

本包不刷新固件、不改相机设置、不发布深度，也不进行视觉伺服或空间几何计算。尚需用实机核对固件、时间同步和图像编码。

## 测试

```bash
source /opt/ros/jazzy/setup.bash
PYTHONPATH=src/insight_camera:$PYTHONPATH python3 -m pytest -q src/insight_camera/test
```

测试使用合成 ROS 消息，不访问真实相机或 API。

## 三图观察

快照 `schema_version=3`，`views` 包含有序 `left/right/rgb` 的相对文件名、话题、`frame_id` 和时间戳。机器人状态与上一动作结果由上层 Codex 任务附加，不由相机节点推断。相机节点只负责采集和保存三路图像。
