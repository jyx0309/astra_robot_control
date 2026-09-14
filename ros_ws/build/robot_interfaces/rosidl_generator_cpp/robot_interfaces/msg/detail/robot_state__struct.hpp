// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/msg/robot_state.hpp"


#ifndef ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_
#define ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'flange_pose'
#include "geometry_msgs/msg/detail/pose__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__robot_interfaces__msg__RobotState __attribute__((deprecated))
#else
# define DEPRECATED__robot_interfaces__msg__RobotState __declspec(deprecated)
#endif

namespace robot_interfaces
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct RobotState_
{
  using Type = RobotState_<ContainerAllocator>;

  explicit RobotState_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init),
    flange_pose(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->connected = false;
      this->enabled = false;
      this->busy = false;
      this->simulated = false;
      this->state_valid = false;
      this->gripper_opening_m = 0.0;
      this->gripper_force_n = 0.0;
      this->controller_state = 0l;
      this->active_command_id = "";
      this->fault = "";
    }
  }

  explicit RobotState_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    flange_pose(_alloc, _init),
    active_command_id(_alloc),
    fault(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->connected = false;
      this->enabled = false;
      this->busy = false;
      this->simulated = false;
      this->state_valid = false;
      this->gripper_opening_m = 0.0;
      this->gripper_force_n = 0.0;
      this->controller_state = 0l;
      this->active_command_id = "";
      this->fault = "";
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _connected_type =
    bool;
  _connected_type connected;
  using _enabled_type =
    bool;
  _enabled_type enabled;
  using _busy_type =
    bool;
  _busy_type busy;
  using _simulated_type =
    bool;
  _simulated_type simulated;
  using _state_valid_type =
    bool;
  _state_valid_type state_valid;
  using _flange_pose_type =
    geometry_msgs::msg::Pose_<ContainerAllocator>;
  _flange_pose_type flange_pose;
  using _gripper_opening_m_type =
    double;
  _gripper_opening_m_type gripper_opening_m;
  using _gripper_force_n_type =
    double;
  _gripper_force_n_type gripper_force_n;
  using _controller_state_type =
    int32_t;
  _controller_state_type controller_state;
  using _active_command_id_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _active_command_id_type active_command_id;
  using _fault_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _fault_type fault;
  using _joint_positions_type =
    std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>>;
  _joint_positions_type joint_positions;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__connected(
    const bool & _arg)
  {
    this->connected = _arg;
    return *this;
  }
  Type & set__enabled(
    const bool & _arg)
  {
    this->enabled = _arg;
    return *this;
  }
  Type & set__busy(
    const bool & _arg)
  {
    this->busy = _arg;
    return *this;
  }
  Type & set__simulated(
    const bool & _arg)
  {
    this->simulated = _arg;
    return *this;
  }
  Type & set__state_valid(
    const bool & _arg)
  {
    this->state_valid = _arg;
    return *this;
  }
  Type & set__flange_pose(
    const geometry_msgs::msg::Pose_<ContainerAllocator> & _arg)
  {
    this->flange_pose = _arg;
    return *this;
  }
  Type & set__gripper_opening_m(
    const double & _arg)
  {
    this->gripper_opening_m = _arg;
    return *this;
  }
  Type & set__gripper_force_n(
    const double & _arg)
  {
    this->gripper_force_n = _arg;
    return *this;
  }
  Type & set__controller_state(
    const int32_t & _arg)
  {
    this->controller_state = _arg;
    return *this;
  }
  Type & set__active_command_id(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->active_command_id = _arg;
    return *this;
  }
  Type & set__fault(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->fault = _arg;
    return *this;
  }
  Type & set__joint_positions(
    const std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> & _arg)
  {
    this->joint_positions = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    robot_interfaces::msg::RobotState_<ContainerAllocator> *;
  using ConstRawPtr =
    const robot_interfaces::msg::RobotState_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::msg::RobotState_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      robot_interfaces::msg::RobotState_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__robot_interfaces__msg__RobotState
    std::shared_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__robot_interfaces__msg__RobotState
    std::shared_ptr<robot_interfaces::msg::RobotState_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const RobotState_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->connected != other.connected) {
      return false;
    }
    if (this->enabled != other.enabled) {
      return false;
    }
    if (this->busy != other.busy) {
      return false;
    }
    if (this->simulated != other.simulated) {
      return false;
    }
    if (this->state_valid != other.state_valid) {
      return false;
    }
    if (this->flange_pose != other.flange_pose) {
      return false;
    }
    if (this->gripper_opening_m != other.gripper_opening_m) {
      return false;
    }
    if (this->gripper_force_n != other.gripper_force_n) {
      return false;
    }
    if (this->controller_state != other.controller_state) {
      return false;
    }
    if (this->active_command_id != other.active_command_id) {
      return false;
    }
    if (this->fault != other.fault) {
      return false;
    }
    if (this->joint_positions != other.joint_positions) {
      return false;
    }
    return true;
  }
  bool operator!=(const RobotState_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct RobotState_

// alias to use template instance with default allocator
using RobotState =
  robot_interfaces::msg::RobotState_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__MSG__DETAIL__ROBOT_STATE__STRUCT_HPP_
