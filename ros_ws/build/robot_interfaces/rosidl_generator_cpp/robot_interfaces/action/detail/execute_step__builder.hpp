// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from robot_interfaces:action/ExecuteStep.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "robot_interfaces/action/execute_step.hpp"


#ifndef ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__BUILDER_HPP_
#define ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "robot_interfaces/action/detail/execute_step__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_Goal_timeout_sec
{
public:
  explicit Init_ExecuteStep_Goal_timeout_sec(::robot_interfaces::action::ExecuteStep_Goal & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_Goal timeout_sec(::robot_interfaces::action::ExecuteStep_Goal::_timeout_sec_type arg)
  {
    msg_.timeout_sec = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

class Init_ExecuteStep_Goal_force_n
{
public:
  explicit Init_ExecuteStep_Goal_force_n(::robot_interfaces::action::ExecuteStep_Goal & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Goal_timeout_sec force_n(::robot_interfaces::action::ExecuteStep_Goal::_force_n_type arg)
  {
    msg_.force_n = std::move(arg);
    return Init_ExecuteStep_Goal_timeout_sec(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

class Init_ExecuteStep_Goal_opening_m
{
public:
  explicit Init_ExecuteStep_Goal_opening_m(::robot_interfaces::action::ExecuteStep_Goal & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Goal_force_n opening_m(::robot_interfaces::action::ExecuteStep_Goal::_opening_m_type arg)
  {
    msg_.opening_m = std::move(arg);
    return Init_ExecuteStep_Goal_force_n(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

class Init_ExecuteStep_Goal_target
{
public:
  explicit Init_ExecuteStep_Goal_target(::robot_interfaces::action::ExecuteStep_Goal & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Goal_opening_m target(::robot_interfaces::action::ExecuteStep_Goal::_target_type arg)
  {
    msg_.target = std::move(arg);
    return Init_ExecuteStep_Goal_opening_m(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

class Init_ExecuteStep_Goal_kind
{
public:
  explicit Init_ExecuteStep_Goal_kind(::robot_interfaces::action::ExecuteStep_Goal & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Goal_target kind(::robot_interfaces::action::ExecuteStep_Goal::_kind_type arg)
  {
    msg_.kind = std::move(arg);
    return Init_ExecuteStep_Goal_target(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

class Init_ExecuteStep_Goal_command_id
{
public:
  Init_ExecuteStep_Goal_command_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_Goal_kind command_id(::robot_interfaces::action::ExecuteStep_Goal::_command_id_type arg)
  {
    msg_.command_id = std::move(arg);
    return Init_ExecuteStep_Goal_kind(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_Goal>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_Goal_command_id();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_Result_state
{
public:
  explicit Init_ExecuteStep_Result_state(::robot_interfaces::action::ExecuteStep_Result & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_Result state(::robot_interfaces::action::ExecuteStep_Result::_state_type arg)
  {
    msg_.state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Result msg_;
};

class Init_ExecuteStep_Result_message
{
public:
  explicit Init_ExecuteStep_Result_message(::robot_interfaces::action::ExecuteStep_Result & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Result_state message(::robot_interfaces::action::ExecuteStep_Result::_message_type arg)
  {
    msg_.message = std::move(arg);
    return Init_ExecuteStep_Result_state(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Result msg_;
};

class Init_ExecuteStep_Result_code
{
public:
  explicit Init_ExecuteStep_Result_code(::robot_interfaces::action::ExecuteStep_Result & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Result_message code(::robot_interfaces::action::ExecuteStep_Result::_code_type arg)
  {
    msg_.code = std::move(arg);
    return Init_ExecuteStep_Result_message(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Result msg_;
};

class Init_ExecuteStep_Result_success
{
public:
  Init_ExecuteStep_Result_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_Result_code success(::robot_interfaces::action::ExecuteStep_Result::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_ExecuteStep_Result_code(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_Result>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_Result_success();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_Feedback_state
{
public:
  explicit Init_ExecuteStep_Feedback_state(::robot_interfaces::action::ExecuteStep_Feedback & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_Feedback state(::robot_interfaces::action::ExecuteStep_Feedback::_state_type arg)
  {
    msg_.state = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Feedback msg_;
};

class Init_ExecuteStep_Feedback_elapsed_sec
{
public:
  explicit Init_ExecuteStep_Feedback_elapsed_sec(::robot_interfaces::action::ExecuteStep_Feedback & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_Feedback_state elapsed_sec(::robot_interfaces::action::ExecuteStep_Feedback::_elapsed_sec_type arg)
  {
    msg_.elapsed_sec = std::move(arg);
    return Init_ExecuteStep_Feedback_state(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Feedback msg_;
};

class Init_ExecuteStep_Feedback_phase
{
public:
  Init_ExecuteStep_Feedback_phase()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_Feedback_elapsed_sec phase(::robot_interfaces::action::ExecuteStep_Feedback::_phase_type arg)
  {
    msg_.phase = std::move(arg);
    return Init_ExecuteStep_Feedback_elapsed_sec(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_Feedback>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_Feedback_phase();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_SendGoal_Request_goal
{
public:
  explicit Init_ExecuteStep_SendGoal_Request_goal(::robot_interfaces::action::ExecuteStep_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_SendGoal_Request goal(::robot_interfaces::action::ExecuteStep_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Request msg_;
};

class Init_ExecuteStep_SendGoal_Request_goal_id
{
public:
  Init_ExecuteStep_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_SendGoal_Request_goal goal_id(::robot_interfaces::action::ExecuteStep_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_ExecuteStep_SendGoal_Request_goal(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_SendGoal_Request>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_SendGoal_Request_goal_id();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_SendGoal_Response_stamp
{
public:
  explicit Init_ExecuteStep_SendGoal_Response_stamp(::robot_interfaces::action::ExecuteStep_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_SendGoal_Response stamp(::robot_interfaces::action::ExecuteStep_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Response msg_;
};

class Init_ExecuteStep_SendGoal_Response_accepted
{
public:
  Init_ExecuteStep_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_SendGoal_Response_stamp accepted(::robot_interfaces::action::ExecuteStep_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_ExecuteStep_SendGoal_Response_stamp(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_SendGoal_Response>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_SendGoal_Response_accepted();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_SendGoal_Event_response
{
public:
  explicit Init_ExecuteStep_SendGoal_Event_response(::robot_interfaces::action::ExecuteStep_SendGoal_Event & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_SendGoal_Event response(::robot_interfaces::action::ExecuteStep_SendGoal_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Event msg_;
};

class Init_ExecuteStep_SendGoal_Event_request
{
public:
  explicit Init_ExecuteStep_SendGoal_Event_request(::robot_interfaces::action::ExecuteStep_SendGoal_Event & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_SendGoal_Event_response request(::robot_interfaces::action::ExecuteStep_SendGoal_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_ExecuteStep_SendGoal_Event_response(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Event msg_;
};

class Init_ExecuteStep_SendGoal_Event_info
{
public:
  Init_ExecuteStep_SendGoal_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_SendGoal_Event_request info(::robot_interfaces::action::ExecuteStep_SendGoal_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_ExecuteStep_SendGoal_Event_request(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_SendGoal_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_SendGoal_Event>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_SendGoal_Event_info();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_GetResult_Request_goal_id
{
public:
  Init_ExecuteStep_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::robot_interfaces::action::ExecuteStep_GetResult_Request goal_id(::robot_interfaces::action::ExecuteStep_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_GetResult_Request>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_GetResult_Request_goal_id();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_GetResult_Response_result
{
public:
  explicit Init_ExecuteStep_GetResult_Response_result(::robot_interfaces::action::ExecuteStep_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_GetResult_Response result(::robot_interfaces::action::ExecuteStep_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Response msg_;
};

class Init_ExecuteStep_GetResult_Response_status
{
public:
  Init_ExecuteStep_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_GetResult_Response_result status(::robot_interfaces::action::ExecuteStep_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_ExecuteStep_GetResult_Response_result(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_GetResult_Response>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_GetResult_Response_status();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_GetResult_Event_response
{
public:
  explicit Init_ExecuteStep_GetResult_Event_response(::robot_interfaces::action::ExecuteStep_GetResult_Event & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_GetResult_Event response(::robot_interfaces::action::ExecuteStep_GetResult_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Event msg_;
};

class Init_ExecuteStep_GetResult_Event_request
{
public:
  explicit Init_ExecuteStep_GetResult_Event_request(::robot_interfaces::action::ExecuteStep_GetResult_Event & msg)
  : msg_(msg)
  {}
  Init_ExecuteStep_GetResult_Event_response request(::robot_interfaces::action::ExecuteStep_GetResult_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_ExecuteStep_GetResult_Event_response(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Event msg_;
};

class Init_ExecuteStep_GetResult_Event_info
{
public:
  Init_ExecuteStep_GetResult_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_GetResult_Event_request info(::robot_interfaces::action::ExecuteStep_GetResult_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_ExecuteStep_GetResult_Event_request(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_GetResult_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_GetResult_Event>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_GetResult_Event_info();
}

}  // namespace robot_interfaces


namespace robot_interfaces
{

namespace action
{

namespace builder
{

class Init_ExecuteStep_FeedbackMessage_feedback
{
public:
  explicit Init_ExecuteStep_FeedbackMessage_feedback(::robot_interfaces::action::ExecuteStep_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::robot_interfaces::action::ExecuteStep_FeedbackMessage feedback(::robot_interfaces::action::ExecuteStep_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_FeedbackMessage msg_;
};

class Init_ExecuteStep_FeedbackMessage_goal_id
{
public:
  Init_ExecuteStep_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_ExecuteStep_FeedbackMessage_feedback goal_id(::robot_interfaces::action::ExecuteStep_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_ExecuteStep_FeedbackMessage_feedback(msg_);
  }

private:
  ::robot_interfaces::action::ExecuteStep_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::robot_interfaces::action::ExecuteStep_FeedbackMessage>()
{
  return robot_interfaces::action::builder::Init_ExecuteStep_FeedbackMessage_goal_id();
}

}  // namespace robot_interfaces

#endif  // ROBOT_INTERFACES__ACTION__DETAIL__EXECUTE_STEP__BUILDER_HPP_
