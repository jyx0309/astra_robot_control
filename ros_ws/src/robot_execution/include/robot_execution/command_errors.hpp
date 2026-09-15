#pragma once
#include <string>
#include <cstdint>
namespace robot_execution {
// Callers serialize SDK callback access. Only the observed IK signature is
// recoverable; other 3001 subcodes must not silently lose their fault semantics.
struct CommandErrors {
  std::string fault, last_command_error;
  uint64_t ik_sequence=0;
  static bool is_ik(int code,const std::string &message) {
    return code==3001 && (message=="[17] unable to cal ik." ||
                         message=="[17] unable to cal ik");
  }
  void receive(int code,const std::string &message) {
    if(code==0 && message.empty()) return;
    const auto detail=std::to_string(code)+":"+message;
    if(is_ik(code,message)) {last_command_error=detail; ++ik_sequence;}
    else if(fault.empty()) fault=detail; // IK cannot overwrite a hardware fault.
  }
};
}
