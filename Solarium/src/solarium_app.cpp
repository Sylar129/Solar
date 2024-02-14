// Copyright (c) 2024 Sylar129

#include "core/base/entrypoint.h"
#include "src/editor_layer.h"

namespace solar {

class Solarium : public Application {
 public:
  Solarium() : Application("Solarium") {
    // PushLayer(new ExampleLayer());
    PushLayer(std::make_shared<EditorLayer>());
  }

  ~Solarium() {}
};

Application* CreateApplication() { return new Solarium(); }

}  // namespace solar
