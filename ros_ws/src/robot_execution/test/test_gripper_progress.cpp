#include <gtest/gtest.h>
#include <robot_execution/gripper_progress.hpp>
using robot_execution::GripperProgress;

TEST(GripperProgress,RecordedOpeningPlateauFailsBeforeActionTimeout) {
  GripperProgress p;
  p.reset(0.009628, 0);
  EXPECT_FALSE(p.stalled(0.025392, 0.04, 0.005, 0.534, 3));
  EXPECT_FALSE(p.stalled(0.032967, 0.04, 0.005, 1.034, 3));
  EXPECT_FALSE(p.stalled(0.032967, 0.04, 0.005, 4.0, 3));
  EXPECT_TRUE(p.stalled(0.032967, 0.04, 0.005, 4.1, 3));
}
TEST(GripperProgress,SlowAccumulatingMotionDoesNotStall) {
  GripperProgress p;
  p.reset(0.01, 0);
  for (int i=1;i<=200;++i)
    EXPECT_FALSE(p.stalled(0.01+i*0.00002, 0.04, 0.005, i*0.05, 3));
}
TEST(GripperProgress,ReachedAndResetDoNotInheritStall) {
  GripperProgress p;
  p.reset(0.01, 0);
  EXPECT_FALSE(p.stalled(0.04, 0.04, 0.005, 10, 3));
  EXPECT_FALSE(p.stalled(0.04, 0.04, 0.005, 20, 3));
  p.reset(0.04, 0);
  EXPECT_FALSE(p.stalled(0.04, 0.06, 0.005, 1, 3));
  EXPECT_TRUE(p.stalled(0.04, 0.06, 0.005, 3, 3));
}
