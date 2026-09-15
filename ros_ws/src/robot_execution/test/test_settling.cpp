#include <gtest/gtest.h>
#include <robot_execution/settling.hpp>
using namespace robot_execution;
TEST(Settling,StateOneDoesNotMeanMeasuredMovement) {
 EXPECT_TRUE(position_controller_state(1));EXPECT_TRUE(position_controller_state(0));
 EXPECT_FALSE(position_controller_state(-1));EXPECT_FALSE(position_controller_state(2));
 Settling s;for(int i=0;i<=30;++i)s.update(i*0.01,{0.1,0.2},{0,0});
 EXPECT_TRUE(s.stationary(0.3));EXPECT_FALSE(s.stationary(1));
}
TEST(Settling,MotionAndMissingHistoryCannotBeCalledStationary) {
 Settling s;s.update(0,{0.1},{0});EXPECT_FALSE(s.stationary(0));
 for(int i=1;i<=40;++i)s.update(i*0.01,{0.1+i*0.001},{0});
 EXPECT_FALSE(s.stationary(0.4));
 for(int i=41;i<=80;++i)s.update(i*0.01,{0.14},{0});
 EXPECT_TRUE(s.stationary(0.8));s.update(0.81,{0.14},{0.1});EXPECT_FALSE(s.stationary(0.81));
}

TEST(Settling,SingleFrameVelocityNoiseDoesNotInvalidateStablePositionWindow) {
 Settling s;
 for(int i=0;i<=40;++i)s.update(i*0.01,{0.1+(i%2)*0.00038},{i%3==0?-0.0366:0.0});
 EXPECT_TRUE(s.stationary(0.4));
 Settling biased;
 for(int i=0;i<=40;++i)biased.update(i*0.01,{0.1},{0.04});
 EXPECT_FALSE(biased.stationary(0.4));
}
