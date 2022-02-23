#include <Solar.h>

class ExampleLayer : public Solar::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {
        // SOLAR_INFO("ExampleLayer::Update");
    }

    void OnEvent(Solar::Event& event) override {
        SOLAR_TRACE("ExampleLayer: {0}", event);
    }
};

class Sandbox : public Solar::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
        PushOverlay(new Solar::ImGuiLayer());
    }

    ~Sandbox() {

    }
};

Solar::Application* Solar::CreateApplication() {
    return new Sandbox();
}
