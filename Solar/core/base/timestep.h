// Copyright (c) 2024 Sylar129

#pragma once

namespace solar {

class TimeStep {
 public:
  explicit TimeStep(float time) : time_(time) {}

  operator float() const { return time_; }

  float GetSeconds() const { return time_; }
  float GetMilliseconds() const { return time_ * 1000.0f; }

 private:
  float time_;
};

}  // namespace solar
