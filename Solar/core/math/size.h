// Copyright (c) 2024 Sylar129

#pragma once

namespace solar {

struct Size {
  union {
    struct {
      float x;
      float y;
    };
    struct {
      float width;
      float height;
    };
  };
};
}  // namespace solar
