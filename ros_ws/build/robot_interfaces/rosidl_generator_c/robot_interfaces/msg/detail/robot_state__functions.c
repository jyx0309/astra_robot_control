// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from robot_interfaces:msg/RobotState.idl
// generated code does not contain a copyright notice
#include "robot_interfaces/msg/detail/robot_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `flange_pose`
#include "geometry_msgs/msg/detail/pose__functions.h"
// Member `active_command_id`
// Member `fault`
#include "rosidl_runtime_c/string_functions.h"
// Member `joint_positions`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
robot_interfaces__msg__RobotState__init(robot_interfaces__msg__RobotState * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    robot_interfaces__msg__RobotState__fini(msg);
    return false;
  }
  // connected
  // enabled
  // busy
  // simulated
  // state_valid
  // flange_pose
  if (!geometry_msgs__msg__Pose__init(&msg->flange_pose)) {
    robot_interfaces__msg__RobotState__fini(msg);
    return false;
  }
  // gripper_opening_m
  // gripper_force_n
  // controller_state
  // active_command_id
  if (!rosidl_runtime_c__String__init(&msg->active_command_id)) {
    robot_interfaces__msg__RobotState__fini(msg);
    return false;
  }
  // fault
  if (!rosidl_runtime_c__String__init(&msg->fault)) {
    robot_interfaces__msg__RobotState__fini(msg);
    return false;
  }
  // joint_positions
  if (!rosidl_runtime_c__double__Sequence__init(&msg->joint_positions, 0)) {
    robot_interfaces__msg__RobotState__fini(msg);
    return false;
  }
  return true;
}

void
robot_interfaces__msg__RobotState__fini(robot_interfaces__msg__RobotState * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // connected
  // enabled
  // busy
  // simulated
  // state_valid
  // flange_pose
  geometry_msgs__msg__Pose__fini(&msg->flange_pose);
  // gripper_opening_m
  // gripper_force_n
  // controller_state
  // active_command_id
  rosidl_runtime_c__String__fini(&msg->active_command_id);
  // fault
  rosidl_runtime_c__String__fini(&msg->fault);
  // joint_positions
  rosidl_runtime_c__double__Sequence__fini(&msg->joint_positions);
}

bool
robot_interfaces__msg__RobotState__are_equal(const robot_interfaces__msg__RobotState * lhs, const robot_interfaces__msg__RobotState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // connected
  if (lhs->connected != rhs->connected) {
    return false;
  }
  // enabled
  if (lhs->enabled != rhs->enabled) {
    return false;
  }
  // busy
  if (lhs->busy != rhs->busy) {
    return false;
  }
  // simulated
  if (lhs->simulated != rhs->simulated) {
    return false;
  }
  // state_valid
  if (lhs->state_valid != rhs->state_valid) {
    return false;
  }
  // flange_pose
  if (!geometry_msgs__msg__Pose__are_equal(
      &(lhs->flange_pose), &(rhs->flange_pose)))
  {
    return false;
  }
  // gripper_opening_m
  if (lhs->gripper_opening_m != rhs->gripper_opening_m) {
    return false;
  }
  // gripper_force_n
  if (lhs->gripper_force_n != rhs->gripper_force_n) {
    return false;
  }
  // controller_state
  if (lhs->controller_state != rhs->controller_state) {
    return false;
  }
  // active_command_id
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->active_command_id), &(rhs->active_command_id)))
  {
    return false;
  }
  // fault
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->fault), &(rhs->fault)))
  {
    return false;
  }
  // joint_positions
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->joint_positions), &(rhs->joint_positions)))
  {
    return false;
  }
  return true;
}

bool
robot_interfaces__msg__RobotState__copy(
  const robot_interfaces__msg__RobotState * input,
  robot_interfaces__msg__RobotState * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // connected
  output->connected = input->connected;
  // enabled
  output->enabled = input->enabled;
  // busy
  output->busy = input->busy;
  // simulated
  output->simulated = input->simulated;
  // state_valid
  output->state_valid = input->state_valid;
  // flange_pose
  if (!geometry_msgs__msg__Pose__copy(
      &(input->flange_pose), &(output->flange_pose)))
  {
    return false;
  }
  // gripper_opening_m
  output->gripper_opening_m = input->gripper_opening_m;
  // gripper_force_n
  output->gripper_force_n = input->gripper_force_n;
  // controller_state
  output->controller_state = input->controller_state;
  // active_command_id
  if (!rosidl_runtime_c__String__copy(
      &(input->active_command_id), &(output->active_command_id)))
  {
    return false;
  }
  // fault
  if (!rosidl_runtime_c__String__copy(
      &(input->fault), &(output->fault)))
  {
    return false;
  }
  // joint_positions
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->joint_positions), &(output->joint_positions)))
  {
    return false;
  }
  return true;
}

robot_interfaces__msg__RobotState *
robot_interfaces__msg__RobotState__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_interfaces__msg__RobotState * msg = (robot_interfaces__msg__RobotState *)allocator.allocate(sizeof(robot_interfaces__msg__RobotState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(robot_interfaces__msg__RobotState));
  bool success = robot_interfaces__msg__RobotState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
robot_interfaces__msg__RobotState__destroy(robot_interfaces__msg__RobotState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    robot_interfaces__msg__RobotState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
robot_interfaces__msg__RobotState__Sequence__init(robot_interfaces__msg__RobotState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_interfaces__msg__RobotState * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(robot_interfaces__msg__RobotState)) {
      return false;
    }
    data = (robot_interfaces__msg__RobotState *)allocator.zero_allocate(size, sizeof(robot_interfaces__msg__RobotState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = robot_interfaces__msg__RobotState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        robot_interfaces__msg__RobotState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
robot_interfaces__msg__RobotState__Sequence__fini(robot_interfaces__msg__RobotState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      robot_interfaces__msg__RobotState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

robot_interfaces__msg__RobotState__Sequence *
robot_interfaces__msg__RobotState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  robot_interfaces__msg__RobotState__Sequence * array = (robot_interfaces__msg__RobotState__Sequence *)allocator.allocate(sizeof(robot_interfaces__msg__RobotState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = robot_interfaces__msg__RobotState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
robot_interfaces__msg__RobotState__Sequence__destroy(robot_interfaces__msg__RobotState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    robot_interfaces__msg__RobotState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
robot_interfaces__msg__RobotState__Sequence__are_equal(const robot_interfaces__msg__RobotState__Sequence * lhs, const robot_interfaces__msg__RobotState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!robot_interfaces__msg__RobotState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
robot_interfaces__msg__RobotState__Sequence__copy(
  const robot_interfaces__msg__RobotState__Sequence * input,
  robot_interfaces__msg__RobotState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(robot_interfaces__msg__RobotState)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(robot_interfaces__msg__RobotState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    robot_interfaces__msg__RobotState * data =
      (robot_interfaces__msg__RobotState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!robot_interfaces__msg__RobotState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          robot_interfaces__msg__RobotState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!robot_interfaces__msg__RobotState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
