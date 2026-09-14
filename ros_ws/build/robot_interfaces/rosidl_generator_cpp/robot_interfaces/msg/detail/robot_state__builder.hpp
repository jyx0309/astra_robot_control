// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/msg/robot_state.hpp"


#ifndef ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
#define ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_interfaces/msg/detail/robot_state__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_interfaces
{

namespace msg
{

namespace builder
{

class Init_RobotState_joint_positions
{
public:
  explicit Init_RobotState_joint_positions(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::msg::RobotState joint_positions(::robot_interfaces::msg::RobotState::_joint_positions_type arg)
  {
    msg_.joint_positions = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_fault
{
public:
  explicit Init_RobotState_fault(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_joint_positions fault(::robot_interfaces::msg::RobotState::_fault_type arg)
  {
    msg_.fault = std::move(arg);
    return Init_RobotState_joint_positions(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_active_command_id
{
public:
  explicit Init_RobotState_active_command_id(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_fault active_command_id(::robot_interfaces::msg::RobotState::_active_command_id_type arg)
  {
    msg_.active_command_id = std::move(arg);
    return Init_RobotState_fault(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_controller_state
{
public:
  explicit Init_RobotState_controller_state(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_active_command_id controller_state(::robot_interfaces::msg::RobotState::_controller_state_type arg)
  {
    msg_.controller_state = std::move(arg);
    return Init_RobotState_active_command_id(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_gripper_force_n
{
public:
  explicit Init_RobotState_gripper_force_n(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_controller_state gripper_force_n(::robot_interfaces::msg::RobotState::_gripper_force_n_type arg)
  {
    msg_.gripper_force_n = std::move(arg);
    return Init_RobotState_controller_state(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_gripper_opening_m
{
public:
  explicit Init_RobotState_gripper_opening_m(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_gripper_force_n gripper_opening_m(::robot_interfaces::msg::RobotState::_gripper_opening_m_type arg)
  {
    msg_.gripper_opening_m = std::move(arg);
    return Init_RobotState_gripper_force_n(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_flange_pose
{
public:
  explicit Init_RobotState_flange_pose(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_gripper_opening_m flange_pose(::robot_interfaces::msg::RobotState::_flange_pose_type arg)
  {
    msg_.flange_pose = std::move(arg);
    return Init_RobotState_gripper_opening_m(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_state_valid
{
public:
  explicit Init_RobotState_state_valid(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_flange_pose state_valid(::robot_interfaces::msg::RobotState::_state_valid_type arg)
  {
    msg_.state_valid = std::move(arg);
    return Init_RobotState_flange_pose(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_simulated
{
public:
  explicit Init_RobotState_simulated(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_state_valid simulated(::robot_interfaces::msg::RobotState::_simulated_type arg)
  {
    msg_.simulated = std::move(arg);
    return Init_RobotState_state_valid(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_busy
{
public:
  explicit Init_RobotState_busy(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_simulated busy(::robot_interfaces::msg::RobotState::_busy_type arg)
  {
    msg_.busy = std::move(arg);
    return Init_RobotState_simulated(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_enabled
{
public:
  explicit Init_RobotState_enabled(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_busy enabled(::robot_interfaces::msg::RobotState::_enabled_type arg)
  {
    msg_.enabled = std::move(arg);
    return Init_RobotState_busy(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_connected
{
public:
  explicit Init_RobotState_connected(::robot_interfaces::msg::RobotState & msg)
  : msg_(msg)
  {}
  Init_RobotState_enabled connected(::robot_interfaces::msg::RobotState::_connected_type arg)
  {
    msg_.connected = std::move(arg);
    return Init_RobotState_enabled(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

class Init_RobotState_header
{
public:
  Init_RobotState_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_RobotState_connected header(::robot_interfaces::msg::RobotState::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_RobotState_connected(msg_);
  }

private:
  ::robot_interfaces::msg::RobotState msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::msg::RobotState>()
{
  return robot_interfaces::msg::builder::Init_RobotState_header();
}

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__BUILDER_HPP_
