#include <Solar.h>
#include <Core/Base/EntryPoint.h>

#include "EditorLayer.h"

namespace Solar {

class Solarium : public Application {
public:
    Solarium() : Application("Solarium")
    {
        // PushLayer(new ExampleLayer());
        PushLayer(std::make_shared<EditorLayer>());
    }

    ~Solarium() {}
};

Application* CreateApplication() { return new Solarium(); }

} // namespace Solar
