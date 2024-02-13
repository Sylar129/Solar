#pragma once

namespace Solar {

class TimeStep {
 public:
  explicit TimeStep(float time = 0.0f) : time_(time) {}

  operator float() const { return time_; }

  float GetSeconds() const { return time_; }
  float GetMilliseconds() const { return time_ * 1000.0f; }

 private:
  float time_;
};

}  // namespace Solar
