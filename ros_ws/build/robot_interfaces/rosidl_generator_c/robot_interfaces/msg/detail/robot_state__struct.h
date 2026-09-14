// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/msg/robot_state.h"


#ifndef ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
#define ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'flange_pose'
#include "geometry_msgs/msg/detail/pose__struct.h"
// Member 'active_command_id'
// Member 'fault'
#include "rosidl_runtime_c/string.h"
// Member 'joint_positions'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/RobotState in the package robot_interfaces.
/**
  * 当前机器人整体状态。
 */
typedef struct robot_interfaces__msg__RobotState
{
  std_msgs__msg__Header header;
  /// 是否已经连接到控制器。
  bool connected;
  /// 是否允许执行运动。
  bool enabled;
  /// 是否正在执行一个动作。
  bool busy;
  /// 是否为模拟后端。
  bool simulated;
  /// 状态数据是否完整、最新且通过基础校验。
  bool state_valid;
  /// base_link 坐标系下的法兰绝对位姿。
  geometry_msgs__msg__Pose flange_pose;
  /// 夹爪开度，单位：米。
  double gripper_opening_m;
  /// 夹爪力，单位：牛顿。
  double gripper_force_n;
  /// 控制器当前状态码。
  int32_t controller_state;
  /// 当前动作的唯一 ID；没有动作时为空。
  rosidl_runtime_c__String active_command_id;
  /// 故障码和故障文本；无故障时为空。
  rosidl_runtime_c__String fault;
  /// 当前关节位置列表。
  rosidl_runtime_c__double__Sequence joint_positions;
} robot_interfaces__msg__RobotState;

// Struct for a sequence of robot_interfaces__msg__RobotState.
typedef struct robot_interfaces__msg__RobotState__Sequence
{
  robot_interfaces__msg__RobotState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__msg__RobotState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_H_
