// Copyright (c) 2024 Sylar129

#pragma once

#include <vector>

#include "core/base/layer.h"
#include "core/utils/misc.h"

namespace solar {

class LayerStack {
 public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Ref<Layer> layer);
  void PushOverlay(Ref<Layer> overlay);
  void PopLayer(Ref<Layer> layer);
  void PopOverlay(Ref<Layer> overlay);

  auto begin() { return layers_.begin(); }
  auto begin() const { return layers_.cbegin(); }

  auto end() { return layers_.end(); }
  auto end() const { return layers_.cend(); }

  auto rbegin() { return layers_.rbegin(); }
  auto rbegin() const { return layers_.crbegin(); }

  auto rend() { return layers_.rend(); }
  auto rend() const { return layers_.crend(); }

 private:
  std::vector<Ref<Layer>> layers_;
  unsigned int layer_insert_index_;
};

}  // namespace solar
