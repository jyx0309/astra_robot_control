#pragma once
#include <cmath>

namespace robot_execution {
// Measure accumulated displacement, not per-sample displacement: slow motion
// must not be mistaken for a stall at a high feedback frequency.
class GripperProgress {
 public:
  void reset(double position, double elapsed) {
    anchor_ = position;
    last_progress_ = elapsed;
  }
  bool stalled(double position, double target, double tolerance,
               double elapsed, double stall_seconds) {
    if (std::abs(position - anchor_) >= 0.0005) reset(position, elapsed);
    if (std::abs(position - target) < tolerance) {
      reset(position, elapsed);
      return false;
    }
    return elapsed - last_progress_ >= stall_seconds;
  }
 private:
  double anchor_ = 0.0;
  double last_progress_ = 0.0;
};
}
