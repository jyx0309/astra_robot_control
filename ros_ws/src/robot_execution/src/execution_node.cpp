#include <arm_control_sdk/carm_cobot.h>
#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <robot_interfaces/action/execute_step.hpp>
#include <robot_interfaces/msg/robot_state.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_srvs/srv/trigger.hpp>
#include <robot_execution/validation.hpp>
#include <robot_execution/gripper_progress.hpp>
#include <chrono>
#include <mutex>
#include <memory>
#include <set>
#include <map>
#include <algorithm>
#include <robot_execution/settling.hpp>
#include <robot_execution/command_errors.hpp>
#include <robot_interfaces/srv/check_step.hpp>
#include <robot_interfaces/srv/query_step.hpp>
using namespace std::chrono_literals;
using robot_execution::Pose;
using Clock=std::chrono::steady_clock;

// 执行节点是 ROS 与 CARM SDK 之间的适配层。
// Codex 只提交绝对末端目标；本节点负责校验、调用 SDK、读取反馈和停止。
// SDK 回调可能在其他线程执行，因此回调写入缓存时必须持有 cache_mutex_。
class ExecutionNode : public rclcpp::Node {
 using Step=robot_interfaces::action::ExecuteStep;
 using Handle=rclcpp_action::ServerGoalHandle<Step>;
 using State=robot_interfaces::msg::RobotState;
public:
 ExecutionNode():Node("robot_execution") {
  // declare_parameter(name, default_value) 会声明 ROS 参数，并返回实际值。
  // 例如启动时传入 -p backend:=carm，就会覆盖这里的 mock 默认值。
  backend_ = declare_parameter("backend", std::string("mock"));
  if(backend_!="mock"&&backend_!="carm") throw std::runtime_error("backend 必须是 mock 或 carm");
  ip_ = declare_parameter("robot_ip", std::string("10.42.0.101"));
  verified_ = declare_parameter("hardware_config_verified", false);
  pose_scale_ = declare_parameter("sdk_pose_units_per_meter", 1.0);
  grip_scale_ = declare_parameter("sdk_gripper_units_per_meter", 1000.0);
  opening_max_ = declare_parameter("max_opening_m", 0.074);
  gripper_tolerance_ = declare_parameter("gripper_position_tolerance_m", 0.005);
  gripper_stall_seconds_ = declare_parameter("gripper_stall_timeout_sec", 3.0);
  force_max_ = declare_parameter("max_force_n", 10.0);
  speed_ = declare_parameter("speed_level", 1.0);
  // Keep the controller/upper-computer collision configuration by default.
  // SDK levels are 0 (most sensitive) through 2 (least sensitive).
  collision_sensitivity_=declare_parameter("collision_sensitivity",-1);
  gripper_topic_=declare_parameter("gripper_topic",std::string("/set_gripper"));
  mock_duration_=declare_parameter("mock_duration_sec",0.3);
  mock_fail_=declare_parameter("mock_fail",false);
  declare_parameter("mock_error",std::string(""));
  // 工作空间来自 ros_ws/src/robot_bringup/config/system.yaml。
  // 这里的六个数依次是 x_min,x_max,y_min,y_max,z_min,z_max，
  // 是本地工程的保守安全边界，不是 CARM SDK 自动计算出的可达空间。
  auto b = declare_parameter(
      "workspace_bounds",
      std::vector<double>{-0.8, 0.8, -0.8, 0.8, 0.02, 1.0});
  if(b.size()!=6) throw std::runtime_error("workspace_bounds 必须包含 6 个值");
  std::copy(b.begin(),b.end(),bounds_.begin());
  for(int i=0;i<3;++i) if(!std::isfinite(bounds_[2*i])||!std::isfinite(bounds_[2*i+1])||bounds_[2*i]>=bounds_[2*i+1]) throw std::runtime_error("工作空间边界无效");
  for(double v:{pose_scale_,grip_scale_,opening_max_,gripper_tolerance_,force_max_,speed_,mock_duration_})
   if(!std::isfinite(v)||v<=0) throw std::runtime_error("正值参数无效");
  if(speed_>10.0||force_max_>20||opening_max_>0.08) throw std::runtime_error("硬件限制无效");
  if(!std::isfinite(gripper_stall_seconds_)||gripper_stall_seconds_<=0) throw std::runtime_error("夹爪停滞超时必须为有限正值");
  pose_ = {0.3, 0., 0.3, 0., 0., 0., 1.};
  enabled_ = backend_ == "mock";

  // create_publisher<MessageType>(topic, queue_depth) 创建一个 ROS 发布器。
  // 发布器只负责“把消息发出去”，消息字段由 RobotState.msg 定义。
  state_pub_ = create_publisher<State>("/robot/state", 10);
  joint_pub_ = create_publisher<sensor_msgs::msg::JointState>(
      "/joint_states", 10);
  gripper_pub_ = create_publisher<sensor_msgs::msg::JointState>(
      gripper_topic_, 10);
  // ROS service 是“请求一次、返回一次”的接口，适合连接、使能和停止。
  // Trigger 没有自定义输入，响应只有 success 和 message。
  connect_srv_ = create_service<std_srvs::srv::Trigger>(
      "/robot/connect",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
             std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
    try {
      if (busy_) {
        throw std::runtime_error("action in progress");
      }
      connect_sdk();
      response->success = true;
      response->message = "Connected; call /robot/enable explicitly before motion";
    } catch (const std::exception &exception) {
      response->success = false;
      response->message = exception.what();
    }
  });

  enable_srv_ = create_service<std_srvs::srv::Trigger>(
      "/robot/enable",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
             std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
   try {
    if (busy_) {
      throw std::runtime_error("action in progress");
    }
    if (backend_ == "carm") {
     if (!verified_) {
       throw std::runtime_error("Confirm firmware, units, workspace, tool and gripper config first");
     }
     // Enabling is the explicit hardware action. Establish the SDK session
     // here too, so a freshly started node does not require a separate
     // /robot/connect call before /robot/enable.
     if (!sdk_ || !sdk_->is_connected()) {
       connect_sdk();
     }
     if (sdk_->set_ready() < 1 ||
         sdk_->set_control_mode(1) < 1 ||
         sdk_->set_speed_level(speed_) < 1) {
      throw std::runtime_error("SDK preparation rejected");
     }
     if (collision_sensitivity_ >= 0 && collision_sensitivity_ <= 2 &&
         sdk_->set_collision_config(true, collision_sensitivity_) < 1) {
      throw std::runtime_error("SDK collision configuration rejected");
     }
    }
    {
      std::lock_guard<std::mutex> lock(cache_mutex_);
      errors_.fault.clear();
    }
    enabled_ = true;
    response->success = true;
    response->message = "准备请求已接受；运动前状态必须有效";
   } catch (const std::exception &exception) {
    enabled_ = false;
    response->success = false;
    response->message = exception.what();
   }
  });
  stop_srv_ = create_service<std_srvs::srv::Trigger>(
      "/robot/stop",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
             std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
   response->success = stop();
   response->message = response->success
                         ? "已请求停止；需要重新使能"
                         : "SDK 停止失败";
   if (active_) {
     finish(false, "stopped", "Stopped by operator");
   }
  });
  if(backend_=="mock") mock_error_srv_=create_service<std_srvs::srv::Trigger>("/robot/mock_ik_event",
   [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,std::shared_ptr<std_srvs::srv::Trigger::Response> res){
    std::lock_guard<std::mutex> l(cache_mutex_);errors_.receive(3001,"[17] unable to cal ik.");res->success=true;
   });
  check_srv_=create_service<robot_interfaces::srv::CheckStep>("/robot/check_step",
   [this](const std::shared_ptr<robot_interfaces::srv::CheckStep::Request> req,
          std::shared_ptr<robot_interfaces::srv::CheckStep::Response> res){
    try {
    const auto check_started=Clock::now();
    refresh(); Step::Goal g;g.command_id="preflight";g.kind=req->kind;g.target=req->target;
    g.opening_m=req->opening_m;g.force_n=req->force_n;g.timeout_sec=req->timeout_sec;
    res->code=validate(g,false);res->valid=res->code.empty();res->message=explain(g,res->code);
    res->state=state_;res->ik_checked=false;res->path_checked=false;
    if(req->check_ik&&res->valid&&g.kind!="set_gripper") {
     if(backend_=="mock") {res->ik_checked=true;res->ik_joint_positions=joints_;}
     else {
      Pose p=target(g);for(int i=0;i<3;++i)p[i]*=pose_scale_;
      std::vector<double> solution;
      uint64_t before;{std::lock_guard<std::mutex> l(cache_mutex_);before=errors_.ik_sequence;}
      const int ret=sdk_->inverse_kine(sdk_->get_tool_index(),p,state_.joint_positions,solution);
      refresh();res->state=state_;
      if(!state_.fault.empty()||!state_.motion_allowed) {res->valid=false;res->code="not_ready";}
      else if(ret<1) {
       res->valid=false;
       std::lock_guard<std::mutex> l(cache_mutex_);
       res->code=errors_.ik_sequence>before?"ik_rejected":"ik_check_unknown";
      } else {
       const bool valid_solution=solution.size()==state_.joint_positions.size()&&!solution.empty()&&
        std::all_of(solution.begin(),solution.end(),[](double v){return std::isfinite(v);});
       res->ik_checked=valid_solution;res->valid=valid_solution;
       if(valid_solution)res->ik_joint_positions=solution;else res->code="ik_check_unknown";
      }
      res->message=res->valid?"Endpoint IK found; path clearance not certified":res->code;
     }
    }
    res->check_sec=std::chrono::duration<double>(Clock::now()-check_started).count();
    } catch(const std::exception &e) {res->valid=false;res->code="check_unavailable";res->message=e.what();}
   });
  query_srv_=create_service<robot_interfaces::srv::QueryStep>("/robot/query_step",
   [this](const std::shared_ptr<robot_interfaces::srv::QueryStep::Request> req,
          std::shared_ptr<robot_interfaces::srv::QueryStep::Response> res){
    try {
    refresh();res->state=state_;
    auto it=results_.find(req->command_id);
    if(it!=results_.end()) {
     res->found=true;res->phase="finished";res->success=it->second.success;
     res->code=it->second.code;res->message=it->second.message;
     res->total_sec=it->second.total_sec;res->sdk_submit_sec=it->second.sdk_submit_sec;
    } else if(seen_.count(req->command_id)) {
     res->found=true;res->phase=issued_?"executing":"accepted";
    } else {res->phase="unknown";res->code="result_unknown";}
    } catch(const std::exception &e) {res->found=false;res->phase="unknown";res->code="result_unknown";res->message=e.what();}
   });
  action_=rclcpp_action::create_server<Step>(this,"/robot/execute_step",
   [this](auto,const std::shared_ptr<const Step::Goal> g){
    try {
    refresh(); auto reason=validate(*g);
    if(!reason.empty()){
     if(!g->command_id.empty()&&!seen_.count(g->command_id)) {
      Step::Result r;r.success=false;r.code=reason;r.message=explain(*g,reason);r.state=state_;
      results_[g->command_id]=r;seen_.insert(g->command_id);
     }
     RCLCPP_WARN(get_logger(),"Rejected %s: %s",g->command_id.c_str(),reason.c_str());
     return rclcpp_action::GoalResponse::REJECT;
    }
    busy_=true; seen_.insert(g->command_id); return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    } catch(const std::exception &e) {RCLCPP_ERROR(get_logger(),"Goal unavailable: %s",e.what());return rclcpp_action::GoalResponse::REJECT;}
   },
   [](auto){return rclcpp_action::CancelResponse::ACCEPT;},
   [this](std::shared_ptr<Handle> h){active_=h; issued_=false; started_=Clock::now();stable_=0;sdk_submit_sec_=0;last_feedback_sec_=-1;ik_pending_=false;submission_unknown_=false;mock_error_injected_=false;
    std::lock_guard<std::mutex> l(cache_mutex_);command_ik_seq_=errors_.ik_sequence;}
  );
  timer_=create_wall_timer(50ms,[this]{tick();});
  RCLCPP_INFO(get_logger(),"Execution backend=%s; real hardware never auto-connects or auto-enables",backend_.c_str());
 }
 ~ExecutionNode() override {
  if(sdk_) {
   if(busy_) sdk_->emergency_stop();
   sdk_->release_pose_cbk();sdk_->release_joint_cbk();
   sdk_->release_error_cbk("robot_execution");sdk_->release_completion_cbk("robot_execution");
   sdk_->disconnect();sdk_.reset();
  }
 }
private:
 void connect_sdk() {
  if(backend_=="mock") return;
  if(sdk_&&sdk_->is_connected()) return;
  if(sdk_) {sdk_->disconnect();sdk_.reset();}
  enabled_=false;
  sdk_=std::make_unique<carm::CArmSingleCol>(ip_,8090,1.0);
  if(!sdk_->is_connected()&&sdk_->connect(ip_,8090,1.0)<1) throw std::runtime_error("SDK 连接失败");
  RCLCPP_INFO(get_logger(), "CARM SDK version: %s", sdk_->get_version().c_str());
  tool_index_=sdk_->get_tool_index();
  auto ec=sdk_->get_eeff_config();
  RCLCPP_INFO(get_logger(), "CARM eeff config: name=%s type=%s dof=%d pos=[%f,%f] vel_max=%f tau_max=%f motor_pos=[%f,%f]",
              ec.eeff_name.c_str(), ec.eeff_type.c_str(), ec.dof,
              ec.limit_lower.empty()?-1.0:ec.limit_lower[0], ec.limit_upper.empty()?-1.0:ec.limit_upper[0],
              ec.eeff_vel.empty()?-1.0:ec.eeff_vel[0], ec.eeff_tau.empty()?-1.0:ec.eeff_tau[0],
              ec.motor_limit_lower.empty()?-1.0:ec.motor_limit_lower[0], ec.motor_limit_upper.empty()?-1.0:ec.motor_limit_upper[0]);
  sdk_->register_pose_cbk([this](double,Pose p){
   std::lock_guard<std::mutex> l(cache_mutex_);
   for(int i=0;i<3;++i) p[i]/=pose_scale_;
   pose_=p;pose_time_=Clock::now();++pose_seq_;
  });
  sdk_->register_joint_cbk([this](double,std::vector<double> p,std::vector<double> v,std::vector<double>){
   std::lock_guard<std::mutex> l(cache_mutex_);joints_=p;joint_vel_=v;joint_time_=Clock::now();
   settling_.update(std::chrono::duration<double>(joint_time_.time_since_epoch()).count(),p,v);
  });
  sdk_->register_error_cbk("robot_execution",[this](int c,std::string m){
   std::lock_guard<std::mutex> l(cache_mutex_);errors_.receive(c,m);
  });
  sdk_->register_completion_cbk("robot_execution",[this](std::string key){
   // SDK does not return a matching task id on submission. Never use this alone as success.
   std::lock_guard<std::mutex> l(cache_mutex_);completion_key_=key;
  });
 }
 void refresh() {
  const auto refresh_started=Clock::now();
  state_.header.stamp=now();state_.header.frame_id="base_link";
  state_.simulated=backend_=="mock";state_.busy=busy_;state_.enabled=enabled_;
  state_.connected=backend_=="mock"||(sdk_&&sdk_->is_connected());
  state_.servo_enabled=backend_=="mock"&&enabled_;
  state_.control_mode=backend_=="mock"?1:0;
  state_.software_motion_enabled=enabled_;
  state_.workspace_bounds=bounds_;state_.speed_level=speed_;
  state_.max_opening_m=opening_max_;state_.max_force_n=force_max_;
  state_.pose_reference="sdk_cartesian_active_tool_unverified";
  state_.tool_index=tool_index_;state_.tool_transform_verified=false;
  int gripper_state=1;
  if(backend_=="carm"&&state_.connected) {
   auto s=sdk_->get_status();state_.controller_state=s.state;
   state_.servo_enabled=s.servo_status;state_.control_mode=s.fsm_state;
   state_.enabled=enabled_&&s.servo_status&&s.fsm_state==1;
   auto ep=sdk_->get_eeff_pos(); auto ev=sdk_->get_eeff_vel(); auto et=sdk_->get_eeff_tau();
   opening_=ep.empty()? -1.0 : ep[0];
   grip_vel_=ev.empty()? 0.0 : ev[0];
   force_=et.empty()? 0.0 : et[0];
   gripper_state=sdk_->get_eeff_state();
  } else state_.controller_state=0;
  std::lock_guard<std::mutex> l(cache_mutex_);
  current_=pose_;state_.fault=errors_.fault;state_.last_command_error=errors_.last_command_error;state_.joint_positions=joints_;
  state_.pose_age_sec=backend_=="mock"?0:std::chrono::duration<double>(Clock::now()-pose_time_).count();
  state_.joint_age_sec=backend_=="mock"?0:std::chrono::duration<double>(Clock::now()-joint_time_).count();
  state_.stationary=backend_=="mock"?(!busy_||ik_pending_):settling_.stationary(std::chrono::duration<double>(Clock::now().time_since_epoch()).count());
  bool finite=!joints_.empty();for(double v:joints_)finite=finite&&std::isfinite(v);for(double v:current_) finite=finite&&std::isfinite(v);
  double q=0;for(int i=3;i<7;++i)q+=current_[i]*current_[i];
  state_.state_valid=finite&&std::abs(q-1)<0.01&&std::isfinite(opening_)&&opening_>=0&&opening_<=0.081&&gripper_state==1&&
   (backend_=="mock"||(Clock::now()-pose_time_<500ms&&Clock::now()-joint_time_<500ms));
  state_.stationary=state_.stationary&&state_.state_valid&&robot_execution::position_controller_state(state_.controller_state);
  state_.motion_allowed=state_.connected&&state_.enabled&&state_.state_valid&&state_.fault.empty()&&state_.stationary&&!busy_;
  state_.sdk_refresh_ms=std::chrono::duration<double,std::milli>(Clock::now()-refresh_started).count();
  if(state_.sdk_refresh_ms>100)RCLCPP_WARN_THROTTLE(get_logger(),*get_clock(),2000,"SDK refresh blocked for %.1f ms",state_.sdk_refresh_ms);
  state_.flange_pose.position.x=current_[0];state_.flange_pose.position.y=current_[1];state_.flange_pose.position.z=current_[2];
  state_.flange_pose.orientation.x=current_[3];state_.flange_pose.orientation.y=current_[4];state_.flange_pose.orientation.z=current_[5];state_.flange_pose.orientation.w=current_[6];
  state_.gripper_opening_m=opening_;state_.gripper_force_n=force_;
  state_.active_command_id=active_?active_->get_goal()->command_id:"";
 }
 static Pose target(const Step::Goal &g) {
  auto &p=g.target.pose;return {p.position.x,p.position.y,p.position.z,p.orientation.x,p.orientation.y,p.orientation.z,p.orientation.w};
 }
 std::string validate(const Step::Goal &g,bool check_id=true) {
  if(busy_)return "busy";
  if(!state_.connected||!state_.enabled||!state_.state_valid)return "not_ready";
  if(!state_.fault.empty()||!robot_execution::position_controller_state(state_.controller_state)||!state_.stationary)return "controller_not_idle";
  if(check_id&&(g.command_id.empty()||seen_.count(g.command_id)))return "empty_or_duplicate_command_id";
  if(!std::isfinite(g.timeout_sec)||g.timeout_sec<0.1||g.timeout_sec>120)return "invalid_timeout";
  if(g.kind=="set_gripper") {
   if(!std::isfinite(g.opening_m)||!std::isfinite(g.force_n)||g.opening_m<0||g.opening_m>opening_max_||g.force_n<=0||g.force_n>force_max_)return "invalid_gripper_target";
   return "";
  }
  if(g.kind!="move_to"&&g.kind!="move_linear")return "unknown_action";
  if(g.target.header.frame_id!="base_link")return "unsupported_frame";
  return robot_execution::validate_pose(target(g),current_,bounds_);
 }
 std::string explain(const Step::Goal &g,const std::string &code) {
  if(code=="outside_workspace") {
   auto p=target(g);std::string out="Target rejected; replan. ";
   for(int i=0;i<3;++i)if(p[i]<bounds_[2*i]||p[i]>bounds_[2*i+1])
    out+=std::string(1,"xyz"[i])+" requested="+std::to_string(p[i])+" allowed=["+
      std::to_string(bounds_[2*i])+","+std::to_string(bounds_[2*i+1])+"] ";
   return out;
  }
  return code.empty()?"Local checks passed; IK and path clearance not certified":code;
 }
 bool stop() {
  enabled_=false;
  if(backend_=="carm"&&sdk_) return sdk_->emergency_stop()>=1;
  return true;
 }
 void finish(bool ok,const std::string &code,const std::string &message) {
  if(!active_) return;
  busy_=false;refresh();state_.active_command_id.clear();
  auto result=std::make_shared<Step::Result>();result->success=ok;result->code=code;result->message=message;result->state=state_;
  result->total_sec=std::chrono::duration<double>(Clock::now()-started_).count();result->sdk_submit_sec=sdk_submit_sec_;
  results_[active_->get_goal()->command_id]=*result;
  if(active_->is_canceling())active_->canceled(result);else if(ok)active_->succeed(result);else active_->abort(result);
  RCLCPP_INFO(get_logger(),"%s: %s",active_->get_goal()->command_id.c_str(),code.c_str());
  active_.reset();
 }
 void tick() {
  try {
   if(backend_=="mock"&&active_&&issued_&&!mock_error_injected_) {
    const auto mode=get_parameter("mock_error").as_string();
    std::lock_guard<std::mutex> l(cache_mutex_);
    if(mode=="ik"||mode=="ik_and_hard") {errors_.receive(3001,"[17] unable to cal ik.");ik_pending_=true;}
    if(mode=="hard"||mode=="ik_and_hard")errors_.receive(9000,"mock hardware fault");
    mock_error_injected_=true;
   }
   refresh();state_pub_->publish(state_);
   sensor_msgs::msg::JointState js;js.header=state_.header;js.position=state_.joint_positions;
   {std::lock_guard<std::mutex> l(cache_mutex_);js.velocity=joint_vel_;}
   if(js.velocity.size()==js.position.size())js.velocity.push_back(grip_vel_/2.0);else js.velocity.clear();
   for(size_t i=0;i<js.position.size();++i)js.name.push_back("joint"+std::to_string(i+1));
   js.name.push_back("gripper_right_joint");js.position.push_back(opening_/2.0);joint_pub_->publish(js);
   if(!active_)return;
   auto g=active_->get_goal();
   if(active_->is_canceling()){stop();finish(false,"canceled","Cancellation requested");return;}
   double elapsed=std::chrono::duration<double>(Clock::now()-started_).count();
   if(elapsed>g->timeout_sec){stop();finish(false,submission_unknown_?"result_unknown":"timeout","Motion deadline reached; stop requested; verify actual state before further motion");return;}
   if(!state_.connected||!state_.state_valid||!state_.enabled||!state_.fault.empty()||state_.controller_state<0){stop();finish(false,"state_fault","Lost fresh/healthy robot state");return;}
   {
    std::lock_guard<std::mutex> l(cache_mutex_);
    if(errors_.ik_sequence>command_ik_seq_&&g->kind!="set_gripper")ik_pending_=true;
   }
   // An asynchronous IK notification is not an emergency stop. Confirm fresh,
   // idle, stationary samples before releasing the serial action slot. The SDK
   // callback lacks task IDs: do not claim exact attribution or auto-resubmit.
   if(ik_pending_) {
    uint64_t seq;{std::lock_guard<std::mutex> l(cache_mutex_);seq=pose_seq_;}
    if(backend_=="mock"||seq!=checked_seq_) {
     checked_seq_=seq;stable_=state_.stationary?stable_+1:0;
    }
    if(stable_>=6)finish(false,"ik_rejected",state_.last_command_error+"; controller idle; obtain a new observation and replan");
    return;
   }
   if(!issued_) {
    const auto submit_started=Clock::now();
    int ret=1;initial_opening_=opening_;last_opening_=opening_;
    gripper_progress_.reset(opening_,elapsed);
    gripper_ack_=1;
    if(backend_=="carm") {
     if(g->kind=="set_gripper") {
      // Prefer the vendor ROS interface when its subscriber is running.
      // Its contract uses meters in position[0] and newtons in effort[0].
      if(gripper_pub_->get_subscription_count()>0) {
       sensor_msgs::msg::JointState cmd;
       cmd.header.stamp=now(); cmd.name={"gripper"};
       cmd.position={g->opening_m}; cmd.effort={g->force_n};
       gripper_pub_->publish(cmd); ret=1;
      } else {
       const bool ec=sdk_->get_eeff_connect(); const auto cfg=sdk_->get_eeff_config();
       const int es=sdk_->get_eeff_state(); const auto ep=sdk_->get_eeff_pos(); const auto et=sdk_->get_eeff_tau();
       const auto pp=sdk_->get_plan_eeff_pos(); const auto pt=sdk_->get_plan_eeff_tau();
       RCLCPP_INFO(get_logger(), "Eeff diagnostic: connect=%s type=%s dof=%d state=%d pos=%f tau=%f plan_pos=%f plan_tau=%f",
                   ec?"true":"false",cfg.eeff_type.c_str(),cfg.dof,es,ep.empty()?-1.0:ep[0],et.empty()?-1.0:et[0],pp.empty()?-1.0:pp[0],pt.empty()?-1.0:pt[0]);
       ret=sdk_->set_eeff({g->opening_m},{0.0},{g->force_n},false);
       const auto pp2=sdk_->get_plan_eeff_pos(); const auto pt2=sdk_->get_plan_eeff_tau();
       RCLCPP_INFO(get_logger(), "Eeff command result=%d plan_pos_after=%f plan_tau_after=%f",ret,pp2.empty()?-1.0:pp2[0],pt2.empty()?-1.0:pt2[0]);
      }
     } else {
      Pose p=target(*g);for(int i=0;i<3;++i)p[i]*=pose_scale_;
      ret=g->kind=="move_linear"?sdk_->move_line_pose(p,false):sdk_->move_pose(p,-1,false);
     }
    }
    if(backend_=="mock"&&get_parameter("mock_error").as_string().find("ack_unknown")==0)ret=-1;
    sdk_submit_sec_=std::chrono::duration<double>(Clock::now()-submit_started).count();
    gripper_ack_=ret;
    refresh();
    if(!state_.connected||!state_.state_valid||!state_.enabled||!state_.fault.empty()||state_.controller_state<0) {
     stop();finish(false,"state_fault","Unhealthy state after SDK submission");return;
    }
    {std::lock_guard<std::mutex> l(cache_mutex_);
     if(errors_.ik_sequence>command_ik_seq_&&g->kind!="set_gripper")ik_pending_=true;
    }
    if(ik_pending_){issued_=true;stable_=0;return;}
    if(ret<1) {
     if(g->kind!="set_gripper") {
      // A negative transport/SDK return alone does not prove non-execution.
      issued_=true;submission_unknown_=true;
      RCLCPP_WARN(get_logger(),"SDK acknowledgement uncertain; monitoring actual target before returning result_unknown");
     }
     // set_eeff is asynchronous on this controller: a negative SDK return
     // can mean the acknowledgement was not decoded even though execution
     // has started. Continue and classify from plan/actual feedback.
     if(g->kind=="set_gripper")RCLCPP_WARN(get_logger(), "set_eeff acknowledgement=%d; waiting for plan/actual feedback", ret);
    }
    issued_=true;
    {std::lock_guard<std::mutex> l(cache_mutex_);issue_seq_=pose_seq_;}
   }
   if(elapsed-last_feedback_sec_>=0.5) {
    auto feedback=std::make_shared<Step::Feedback>();feedback->phase="EXECUTING";
    feedback->elapsed_sec=elapsed;feedback->state=state_;active_->publish_feedback(feedback);last_feedback_sec_=elapsed;
   }
   if(backend_=="mock") {
    if(mock_fail_){stop();finish(false,"mock_failure","Injected failure");return;}
    if(get_parameter("mock_error").as_string()=="ack_unknown_timeout")return;
    if(elapsed<mock_duration_)return;
    if(g->kind=="set_gripper"){opening_=g->opening_m;force_=g->force_n;}else {std::lock_guard<std::mutex> l(cache_mutex_);pose_=target(*g);}
    finish(true,"completed","模拟运动完成");return;
   }
   bool stationary=state_.stationary;uint64_t sequence;
   {std::lock_guard<std::mutex> l(cache_mutex_);sequence=pose_seq_;}
   // Require fresh post-command samples and measured settling, not just SDK acknowledgement.
   if(sequence<=issue_seq_||sequence==checked_seq_)return;
   checked_seq_=sequence;
   bool done=false,contact=false;
   if(g->kind=="set_gripper") {
    bool settled=std::abs(opening_-last_opening_)<0.0005&&std::abs(grip_vel_)<0.002;
    bool reached=std::abs(opening_-g->opening_m)<gripper_tolerance_;
    contact=g->opening_m<initial_opening_&&opening_<initial_opening_-0.001&&settled;
    // Early closing can represent contact, but never proves a grasp.
    // Opening outside tolerance must fail promptly even when the SDK reports
    // healthy state; do not disguise it by widening the position tolerance.
    if(gripper_progress_.stalled(opening_,g->opening_m,gripper_tolerance_,elapsed,gripper_stall_seconds_)&&!contact) {
     const auto planned=sdk_?sdk_->get_plan_eeff_pos():std::vector<double>{};
     const auto planned_force=sdk_?sdk_->get_plan_eeff_tau():std::vector<double>{};
     const std::string detail="Gripper made no positional progress; target_m="+std::to_string(g->opening_m)+
       " actual_m="+std::to_string(opening_)+" measured_force_n="+std::to_string(force_)+
       " requested_force_n="+std::to_string(g->force_n)+" sdk_ack="+std::to_string(gripper_ack_)+
       " plan_m="+(planned.empty()?"unavailable":std::to_string(planned[0]))+
       " plan_force_n="+(planned_force.empty()?"unavailable":std::to_string(planned_force[0]));
     stop();finish(false,"gripper_stalled",detail);return;
    }
    done=settled&&(reached||contact)&&elapsed>0.5;
    last_opening_=opening_;
   } else done=state_.controller_state>=0&&stationary&&robot_execution::distance(current_,target(*g))<0.003&&robot_execution::angle(current_,target(*g))<0.03;
   stable_=done?stable_+1:0;
   if(stable_>=6)finish(true,contact?"gripper_settled":"completed",contact?"闭合停稳；可能是接触或自身停滞，必须通过视觉试抬验证抓取":"已到达目标且运动稳定");
  } catch(const std::exception &e) {
   stop(); if(active_)finish(false,"exception",e.what());else RCLCPP_ERROR_THROTTLE(get_logger(),*get_clock(),2000,"%s",e.what());
  }
 }
 std::string backend_,ip_,completion_key_,gripper_topic_;
 robot_execution::CommandErrors errors_;
 robot_execution::Settling settling_;
 uint64_t command_ik_seq_=0;
 bool ik_pending_=false,submission_unknown_=false,mock_error_injected_=false;
 double sdk_submit_sec_=0,last_feedback_sec_=-1;
 int tool_index_=-1;
 std::map<std::string,Step::Result> results_;
 rclcpp::Service<robot_interfaces::srv::CheckStep>::SharedPtr check_srv_;
 rclcpp::Service<robot_interfaces::srv::QueryStep>::SharedPtr query_srv_;
 bool enabled_=false,verified_=false,busy_=false,issued_=false,mock_fail_=false;
 int collision_sensitivity_=-1;
 double pose_scale_,grip_scale_,opening_max_,gripper_tolerance_,force_max_,speed_,mock_duration_;
 double opening_=0.05,force_=0,grip_vel_=0,initial_opening_=0,last_opening_=0;
 robot_execution::GripperProgress gripper_progress_;
 double gripper_stall_seconds_=3.0;
 int gripper_ack_=1;
 int stable_=0;uint64_t pose_seq_=0,issue_seq_=0,checked_seq_=0;
 std::array<double,6> bounds_;Pose pose_,current_;
 std::vector<double> joints_=std::vector<double>(6,0.),joint_vel_;
 std::mutex cache_mutex_;Clock::time_point pose_time_{},joint_time_{},started_{};
 std::set<std::string> seen_;State state_;
 std::unique_ptr<carm::CArmSingleCol> sdk_;
 std::shared_ptr<Handle> active_;
 rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr mock_error_srv_;
 rclcpp_action::Server<Step>::SharedPtr action_;
 rclcpp::Publisher<State>::SharedPtr state_pub_;
 rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_pub_,gripper_pub_;
 rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr connect_srv_,enable_srv_,stop_srv_;
 rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc,char **argv){rclcpp::init(argc,argv);auto node=std::make_shared<ExecutionNode>();rclcpp::spin(node);node.reset();rclcpp::shutdown();return 0;}
