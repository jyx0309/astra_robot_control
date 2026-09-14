#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to robot_interfaces__msg__RobotState
/// 当前机器人整体状态。

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct RobotState {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 是否已经连接到控制器。
    pub connected: bool,

    /// 是否允许执行运动。
    pub enabled: bool,

    /// 是否正在执行一个动作。
    pub busy: bool,

    /// 是否为模拟后端。
    pub simulated: bool,

    /// 状态数据是否完整、最新且通过基础校验。
    pub state_valid: bool,

    /// base_link 坐标系下的法兰绝对位姿。
    pub flange_pose: geometry_msgs::msg::Pose,

    /// 夹爪开度，单位：米。
    pub gripper_opening_m: f64,

    /// 夹爪力，单位：牛顿。
    pub gripper_force_n: f64,

    /// 控制器当前状态码。
    pub controller_state: i32,

    /// 当前动作的唯一 ID；没有动作时为空。
    pub active_command_id: std::string::String,

    /// 故障码和故障文本；无故障时为空。
    pub fault: std::string::String,

    /// 当前关节位置列表。
    pub joint_positions: Vec<f64>,

}



impl Default for RobotState {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::RobotState::default())
  }
}

impl rosidl_runtime_rs::Message for RobotState {
  type RmwMsg = super::msg::rmw::RobotState;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        connected: msg.connected,
        enabled: msg.enabled,
        busy: msg.busy,
        simulated: msg.simulated,
        state_valid: msg.state_valid,
        flange_pose: geometry_msgs::msg::Pose::into_rmw_message(std::borrow::Cow::Owned(msg.flange_pose)).into_owned(),
        gripper_opening_m: msg.gripper_opening_m,
        gripper_force_n: msg.gripper_force_n,
        controller_state: msg.controller_state,
        active_command_id: msg.active_command_id.as_str().into(),
        fault: msg.fault.as_str().into(),
        joint_positions: msg.joint_positions.into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
      connected: msg.connected,
      enabled: msg.enabled,
      busy: msg.busy,
      simulated: msg.simulated,
      state_valid: msg.state_valid,
        flange_pose: geometry_msgs::msg::Pose::into_rmw_message(std::borrow::Cow::Borrowed(&msg.flange_pose)).into_owned(),
      gripper_opening_m: msg.gripper_opening_m,
      gripper_force_n: msg.gripper_force_n,
      controller_state: msg.controller_state,
        active_command_id: msg.active_command_id.as_str().into(),
        fault: msg.fault.as_str().into(),
        joint_positions: msg.joint_positions.as_slice().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      connected: msg.connected,
      enabled: msg.enabled,
      busy: msg.busy,
      simulated: msg.simulated,
      state_valid: msg.state_valid,
      flange_pose: geometry_msgs::msg::Pose::from_rmw_message(msg.flange_pose),
      gripper_opening_m: msg.gripper_opening_m,
      gripper_force_n: msg.gripper_force_n,
      controller_state: msg.controller_state,
      active_command_id: msg.active_command_id.to_string(),
      fault: msg.fault.to_string(),
      joint_positions: msg.joint_positions
          .into_iter()
          .collect(),
    }
  }
}


