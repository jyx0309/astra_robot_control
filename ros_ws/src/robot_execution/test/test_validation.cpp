#include <gtest/gtest.h>
#include <limits>
#include <robot_execution/validation.hpp>
using namespace robot_execution;
TEST(Validation,RejectBadTargets){Pose p{0.3,0,0.3,0,0,0,1},c=p;std::array<double,6>b{-0.6,0.6,-0.6,0.6,0.02,0.8};
EXPECT_EQ(validate_pose(p,c,b),"");p[0]=0.5;EXPECT_EQ(validate_pose(p,c,b),"");p=c;p[2]=-0.1;EXPECT_EQ(validate_pose(p,c,b),"outside_workspace");p=c;p[6]=0;EXPECT_EQ(validate_pose(p,c,b),"quaternion_not_normalized");p=c;p[0]=std::numeric_limits<double>::quiet_NaN();EXPECT_EQ(validate_pose(p,c,b),"nonfinite_pose");}
TEST(Validation,QuaternionSignIsEquivalent){Pose a{0,0,0,0,0,0,1},b{0,0,0,0,0,0,-1};EXPECT_DOUBLE_EQ(angle(a,b),0);}

TEST(Validation,DefaultAllowsLargePlannerSelectedStepWithinWorkspace) {
 Pose current{0.3,0,0.3,0,0,0,1},target{-0.5,0.4,0.7,1,0,0,0};
 std::array<double,6> bounds{-0.6,0.6,-0.6,0.6,0.02,0.8};
 EXPECT_EQ(validate_pose(target,current,bounds),"");
 target[0]=0.7;EXPECT_EQ(validate_pose(target,current,bounds),"outside_workspace");
}
