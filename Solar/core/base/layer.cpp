// Copyright (c) 2024 Sylar129

#include "core/base/layer.h"

namespace solar {

Layer::Layer(const std::string& debug_name) : debug_name_(debug_name) {}

Layer::~Layer() {}

std::string Layer::GetDebugName() const { return debug_name_; }

}  // namespace solar
