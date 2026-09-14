#pragma once
#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
namespace robot_execution {
using Pose = std::array<double, 7>;
inline double distance(const Pose &a, const Pose &b) {
 // 只比较位姿中的位置部分，单位为米。
 double s=0; for(int i=0;i<3;++i) s+=(a[i]-b[i])*(a[i]-b[i]); return std::sqrt(s);
}
inline double angle(const Pose &a, const Pose &b) {
 // 四元数点积给出两姿态之间的最小旋转角。
 double dot=0; for(int i=3;i<7;++i) dot+=a[i]*b[i];
 return 2*std::acos(std::min(1.0,std::abs(dot)));
}
inline std::string validate_pose(const Pose &p,const Pose &current,
 const std::array<double,6> &bounds,double max_step,double max_angle) {
 // 这是 SDK 调用前的本地安全闸门，不负责求 IK。
 for(double x:p) if(!std::isfinite(x)) return "nonfinite_pose";
 double q=0; for(int i=3;i<7;++i) q+=p[i]*p[i];
 if(std::abs(q-1)>0.001) return "quaternion_not_normalized";
 for(int i=0;i<3;++i) if(p[i]<bounds[2*i]||p[i]>bounds[2*i+1]) return "outside_workspace";
 if(distance(p,current)>max_step) return "translation_step_too_large";
 if(angle(p,current)>max_angle) return "rotation_step_too_large";
 return "";
}
}
