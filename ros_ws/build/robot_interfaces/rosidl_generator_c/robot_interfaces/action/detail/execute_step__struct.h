// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from robot_interfaces:action/ExecuteStep.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/action/execute_step.h"


#ifndef ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__STRUCT_H_
#define ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'command_id'
// Member 'kind'
#include "rosidl_runtime_c/string.h"
// Member 'target'
#include "geometry_msgs/msg/detail/pose_stamped__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_Goal
{
  rosidl_runtime_c__String command_id;
  rosidl_runtime_c__String kind;
  geometry_msgs__msg__PoseStamped target;
  double opening_m;
  double force_n;
  double timeout_sec;
} robot_interfaces__action__ExecuteStep_Goal;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_Goal.
typedef struct robot_interfaces__action__ExecuteStep_Goal__Sequence
{
  robot_interfaces__action__ExecuteStep_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_Goal__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'code'
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"
// Member 'state'
#include "robot_interfaces/msg/detail/robot_state__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_Result
{
  bool success;
  rosidl_runtime_c__String code;
  rosidl_runtime_c__String message;
  robot_interfaces__msg__RobotState state;
} robot_interfaces__action__ExecuteStep_Result;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_Result.
typedef struct robot_interfaces__action__ExecuteStep_Result__Sequence
{
  robot_interfaces__action__ExecuteStep_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_Result__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'phase'
// already included above
// #include "rosidl_runtime_c/string.h"
// Member 'state'
// already included above
// #include "robot_interfaces/msg/detail/robot_state__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_Feedback
{
  rosidl_runtime_c__String phase;
  double elapsed_sec;
  robot_interfaces__msg__RobotState state;
} robot_interfaces__action__ExecuteStep_Feedback;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_Feedback.
typedef struct robot_interfaces__action__ExecuteStep_Feedback__Sequence
{
  robot_interfaces__action__ExecuteStep_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_Feedback__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "robot_interfaces/action/detail/execute_step__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  robot_interfaces__action__ExecuteStep_Goal goal;
} robot_interfaces__action__ExecuteStep_SendGoal_Request;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_SendGoal_Request.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Request__Sequence
{
  robot_interfaces__action__ExecuteStep_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_SendGoal_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} robot_interfaces__action__ExecuteStep_SendGoal_Response;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_SendGoal_Response.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Response__Sequence
{
  robot_interfaces__action__ExecuteStep_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_SendGoal_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  robot_interfaces__action__ExecuteStep_SendGoal_Event__request__MAX_SIZE = 1
};
// response
enum
{
  robot_interfaces__action__ExecuteStep_SendGoal_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Event
{
  service_msgs__msg__ServiceEventInfo info;
  robot_interfaces__action__ExecuteStep_SendGoal_Request__Sequence request;
  robot_interfaces__action__ExecuteStep_SendGoal_Response__Sequence response;
} robot_interfaces__action__ExecuteStep_SendGoal_Event;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_SendGoal_Event.
typedef struct robot_interfaces__action__ExecuteStep_SendGoal_Event__Sequence
{
  robot_interfaces__action__ExecuteStep_SendGoal_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_SendGoal_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} robot_interfaces__action__ExecuteStep_GetResult_Request;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_GetResult_Request.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Request__Sequence
{
  robot_interfaces__action__ExecuteStep_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_GetResult_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "robot_interfaces/action/detail/execute_step__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Response
{
  int8_t status;
  robot_interfaces__action__ExecuteStep_Result result;
} robot_interfaces__action__ExecuteStep_GetResult_Response;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_GetResult_Response.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Response__Sequence
{
  robot_interfaces__action__ExecuteStep_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_GetResult_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  robot_interfaces__action__ExecuteStep_GetResult_Event__request__MAX_SIZE = 1
};
// response
enum
{
  robot_interfaces__action__ExecuteStep_GetResult_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Event
{
  service_msgs__msg__ServiceEventInfo info;
  robot_interfaces__action__ExecuteStep_GetResult_Request__Sequence request;
  robot_interfaces__action__ExecuteStep_GetResult_Response__Sequence response;
} robot_interfaces__action__ExecuteStep_GetResult_Event;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_GetResult_Event.
typedef struct robot_interfaces__action__ExecuteStep_GetResult_Event__Sequence
{
  robot_interfaces__action__ExecuteStep_GetResult_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_GetResult_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "robot_interfaces/action/detail/execute_step__struct.h"

/// Struct defined in action/ExecuteStep in the package robot_interfaces.
typedef struct robot_interfaces__action__ExecuteStep_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  robot_interfaces__action__ExecuteStep_Feedback feedback;
} robot_interfaces__action__ExecuteStep_FeedbackMessage;

// Struct for a sequence of robot_interfaces__action__ExecuteStep_FeedbackMessage.
typedef struct robot_interfaces__action__ExecuteStep_FeedbackMessage__Sequence
{
  robot_interfaces__action__ExecuteStep_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} robot_interfaces__action__ExecuteStep_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__STRUCT_H_
