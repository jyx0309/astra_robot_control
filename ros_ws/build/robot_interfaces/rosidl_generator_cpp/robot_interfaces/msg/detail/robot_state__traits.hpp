// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/msg/robot_state.hpp"


#ifndef ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
#define ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "robot_interfaces/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'flange_pose'
#include "geometry_msgs/msg/detail/pose__traits.hpp"

namespace robot_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const RobotState & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: connected
  {
    out << "connected: ";
    rosidl_generator_traits::value_to_yaml(msg.connected, out);
    out << ", ";
  }

  // member: enabled
  {
    out << "enabled: ";
    rosidl_generator_traits::value_to_yaml(msg.enabled, out);
    out << ", ";
  }

  // member: busy
  {
    out << "busy: ";
    rosidl_generator_traits::value_to_yaml(msg.busy, out);
    out << ", ";
  }

  // member: simulated
  {
    out << "simulated: ";
    rosidl_generator_traits::value_to_yaml(msg.simulated, out);
    out << ", ";
  }

  // member: state_valid
  {
    out << "state_valid: ";
    rosidl_generator_traits::value_to_yaml(msg.state_valid, out);
    out << ", ";
  }

  // member: flange_pose
  {
    out << "flange_pose: ";
    to_flow_style_yaml(msg.flange_pose, out);
    out << ", ";
  }

  // member: gripper_opening_m
  {
    out << "gripper_opening_m: ";
    rosidl_generator_traits::value_to_yaml(msg.gripper_opening_m, out);
    out << ", ";
  }

  // member: gripper_force_n
  {
    out << "gripper_force_n: ";
    rosidl_generator_traits::value_to_yaml(msg.gripper_force_n, out);
    out << ", ";
  }

  // member: controller_state
  {
    out << "controller_state: ";
    rosidl_generator_traits::value_to_yaml(msg.controller_state, out);
    out << ", ";
  }

  // member: active_command_id
  {
    out << "active_command_id: ";
    rosidl_generator_traits::value_to_yaml(msg.active_command_id, out);
    out << ", ";
  }

  // member: fault
  {
    out << "fault: ";
    rosidl_generator_traits::value_to_yaml(msg.fault, out);
    out << ", ";
  }

  // member: joint_positions
  {
    if (msg.joint_positions.size() == 0) {
      out << "joint_positions: []";
    } else {
      out << "joint_positions: [";
      size_t pending_items = msg.joint_positions.size();
      for (auto item : msg.joint_positions) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const RobotState & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: connected
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "connected: ";
    rosidl_generator_traits::value_to_yaml(msg.connected, out);
    out << "\n";
  }

  // member: enabled
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "enabled: ";
    rosidl_generator_traits::value_to_yaml(msg.enabled, out);
    out << "\n";
  }

  // member: busy
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "busy: ";
    rosidl_generator_traits::value_to_yaml(msg.busy, out);
    out << "\n";
  }

  // member: simulated
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "simulated: ";
    rosidl_generator_traits::value_to_yaml(msg.simulated, out);
    out << "\n";
  }

  // member: state_valid
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "state_valid: ";
    rosidl_generator_traits::value_to_yaml(msg.state_valid, out);
    out << "\n";
  }

  // member: flange_pose
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "flange_pose:\n";
    to_block_style_yaml(msg.flange_pose, out, indentation + 2);
  }

  // member: gripper_opening_m
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "gripper_opening_m: ";
    rosidl_generator_traits::value_to_yaml(msg.gripper_opening_m, out);
    out << "\n";
  }

  // member: gripper_force_n
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "gripper_force_n: ";
    rosidl_generator_traits::value_to_yaml(msg.gripper_force_n, out);
    out << "\n";
  }

  // member: controller_state
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "controller_state: ";
    rosidl_generator_traits::value_to_yaml(msg.controller_state, out);
    out << "\n";
  }

  // member: active_command_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "active_command_id: ";
    rosidl_generator_traits::value_to_yaml(msg.active_command_id, out);
    out << "\n";
  }

  // member: fault
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "fault: ";
    rosidl_generator_traits::value_to_yaml(msg.fault, out);
    out << "\n";
  }

  // member: joint_positions
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.joint_positions.size() == 0) {
      out << "joint_positions: []\n";
    } else {
      out << "joint_positions:\n";
      for (auto item : msg.joint_positions) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const RobotState & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace robot_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use robot_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const robot_interfaces::msg::RobotState & msg,
  std::ostream & out, size_t indentation = 0)
{
  robot_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use robot_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const robot_interfaces::msg::RobotState & msg)
{
  return robot_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<robot_interfaces::msg::RobotState>()
{
  return "robot_interfaces::msg::RobotState";
}

template<>
inline const char * name<robot_interfaces::msg::RobotState>()
{
  return "robot_interfaces/msg/RobotState";
}

template<>
struct has_fixed_size<robot_interfaces::msg::RobotState>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<robot_interfaces::msg::RobotState>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<robot_interfaces::msg::RobotState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__TRAITS_HPP_
