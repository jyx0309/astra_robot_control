#include <gtest/gtest.h>
#include <robot_execution/command_errors.hpp>
using robot_execution::CommandErrors;
TEST(CommandErrors,RecordedIKIsRecoverableButOther3001IsNot) {
 CommandErrors e;e.receive(3001,"[17] unable to cal ik.");
 EXPECT_TRUE(e.fault.empty());EXPECT_EQ(e.ik_sequence,1u);
 e.receive(3001,"[18] other error");EXPECT_FALSE(e.fault.empty());
}
TEST(CommandErrors,IKNeverClearsHardwareFaultAndEmptyCallbackDoesNotClearIt) {
 CommandErrors e;e.receive(9000,"collision");
 e.receive(3001,"[17] unable to cal ik.");e.receive(0,"");
 EXPECT_EQ(e.fault,"9000:collision");
}
TEST(CommandErrors,RepeatedErrorsAreObservableWithoutLatchingFault) {
 CommandErrors e;e.receive(3001,"[17] unable to cal ik.");auto boundary=e.ik_sequence;
 EXPECT_FALSE(e.ik_sequence>boundary);
 e.receive(3001,"[17] unable to cal ik.");EXPECT_GT(e.ik_sequence,boundary);
 EXPECT_TRUE(e.fault.empty());
}
