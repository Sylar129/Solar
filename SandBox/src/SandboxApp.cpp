#include <Solar.h>

#include "imgui/imgui.h"

class ExampleLayer : public Solar::Layer {
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override {
        // SOLAR_INFO("ExampleLayer::Update");
        if (Solar::Input::IsKeyPressed(SOLAR_KEY_TAB)) {
            SOLAR_INFO("Tab key is pressed!");
        }
    }

    void OnEvent(Solar::Event& event) override {
        // SOLAR_TRACE("ExampleLayer: {0}", event);
    }

    void OnImGuiRender() override {
        ImGui::Begin("Example");
        ImGui::Text("Hello world");
        ImGui::End();
    }
};

class Sandbox : public Solar::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }
};

Solar::Application* Solar::CreateApplication() {
    return new Sandbox();
}
