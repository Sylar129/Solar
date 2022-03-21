#include <Solar.h>
#include <Solar/Core/EntryPoint.h>

#include "ExampleLayer.h"
#include "Sandbox2D.h"

class Sandbox : public Solar::Application {
public:
    Sandbox() {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox() {

    }
};

Solar::Application* Solar::CreateApplication() {
    return new Sandbox();
}
