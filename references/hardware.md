# 硬件预检

在操作员确认工作空间无障碍且控制器配置已验证之前，不要使能电机。

## 网络与控制器

```bash
ip -br link
cat /sys/class/net/enp129s0/carrier
ip -br addr show dev enp129s0
ping -c 3 10.42.0.101
```

确认控制器 IP 和端口后，才能启动 CARM 后端：

```bash
cd /path/to/astra_robot_control/ros_ws
source vendor/arm_control_sdk/setup.bash
ros2 run robot_execution execution_node --ros-args \
  -p backend:=carm -p robot_ip:=10.42.0.101 \
  -p hardware_config_verified:=true
ros2 service call /robot/enable std_srvs/srv/Trigger "{}"
```

当前执行器使用 CARM SDK 8090 端口和 POSITION 模式（`set_control_mode(1)`），并使用 SDK 内部 IK。MVP 不包含自定义 IK、MoveIt、相机几何、TF 或 RGB-D 处理。

真实硬件启动前，操作员仍须确认控制器固件、单位、工作空间、工具和夹爪配置。所有运动限制、工作空间校验、IK 响应、碰撞/故障处理和停止动作均由 `robot_execution` 负责。

相机轴映射、手眼标定、相机到工具 TF、深度对齐和像素到 3D 的转换不属于 MVP；如需使用，应放在后续 enhancement 中单独设计和验证。
