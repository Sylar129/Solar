// Copyright (c) 2024 Sylar129

#include <Solar.h>

#include "Core/Base/EntryPoint.h"
#include "EditorLayer.h"

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
