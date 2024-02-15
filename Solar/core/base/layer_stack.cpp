// Copyright (c) 2024 Sylar129

#include "core/base/layer_stack.h"

namespace solar {

LayerStack::LayerStack() : layer_insert_index_(0) {}

LayerStack::~LayerStack() {
  for (auto& layer : layers_) {
    layer->OnDetach();
  }
}

void LayerStack::PushLayer(Ref<Layer> layer) {
  layers_.emplace(layers_.begin() + layer_insert_index_, layer);
  layer_insert_index_++;
}

void LayerStack::PushOverlay(Ref<Layer> overlay) {
  layers_.emplace_back(overlay);
}

void LayerStack::PopLayer(Ref<Layer> layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);
  if (it != layers_.end()) {
    layer->OnDetach();
    layers_.erase(it);
    layer_insert_index_--;
  }
}

void LayerStack::PopOverlay(Ref<Layer> overlay) {
  auto it = std::find(layers_.begin(), layers_.end(), overlay);
  if (it != layers_.end()) {
    overlay->OnDetach();
    layers_.erase(it);
  }
}

}  // namespace solar
