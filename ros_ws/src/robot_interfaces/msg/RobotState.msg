# 当前机器人整体状态。
std_msgs/Header header
# 是否已经连接到控制器。
bool connected
# 是否允许执行运动。
bool enabled
# 是否正在执行一个动作。
bool busy
# 是否为模拟后端。
bool simulated
# 状态数据是否完整、最新且通过基础校验。
bool state_valid
# base_link 坐标系下的法兰绝对位姿。
geometry_msgs/Pose flange_pose
# 夹爪开度，单位：米。
float64 gripper_opening_m
# 夹爪力，单位：牛顿。
float64 gripper_force_n
# 控制器当前状态码。
int32 controller_state
# 当前动作的唯一 ID；没有动作时为空。
string active_command_id
# 故障码和故障文本；无故障时为空。
string fault
# 当前关节位置列表。
float64[] joint_positions
# 原始伺服状态仅在 connected 和 state_valid 时可解释。
bool servo_enabled
int32 control_mode
bool software_motion_enabled
bool motion_allowed
bool stationary
float64 pose_age_sec
float64 joint_age_sec
float64[6] workspace_bounds
float64 speed_level
float64 max_opening_m
float64 max_force_n
# SDK Cartesian pose；历史字段 flange_pose 不保证是物理法兰或轴尖。
string pose_reference
int32 tool_index
bool tool_transform_verified
# 最近命令错误是诊断信息，不是持久硬故障。
string last_command_error
float64 sdk_refresh_ms
