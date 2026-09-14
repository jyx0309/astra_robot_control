#include <gtest/gtest.h>
#include <limits>
#include <robot_execution/validation.hpp>
using namespace robot_execution;
TEST(Validation,RejectBadTargets){Pose p{0.3,0,0.3,0,0,0,1},c=p;std::array<double,6>b{-0.6,0.6,-0.6,0.6,0.02,0.8};
EXPECT_EQ(validate_pose(p,c,b,0.05,0.35),"");p[0]=0.5;EXPECT_EQ(validate_pose(p,c,b,0.05,0.35),"translation_step_too_large");p=c;p[2]=-0.1;EXPECT_EQ(validate_pose(p,c,b,0.05,0.35),"outside_workspace");p=c;p[6]=0;EXPECT_EQ(validate_pose(p,c,b,0.05,0.35),"quaternion_not_normalized");p=c;p[0]=std::numeric_limits<double>::quiet_NaN();EXPECT_EQ(validate_pose(p,c,b,0.05,0.35),"nonfinite_pose");}
TEST(Validation,QuaternionSignIsEquivalent){Pose a{0,0,0,0,0,0,1},b{0,0,0,0,0,0,-1};EXPECT_DOUBLE_EQ(angle(a,b),0);}
