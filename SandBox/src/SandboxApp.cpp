#include <Solar.h>

class Sandbox : public Solar::Application {
public:
    Sandbox() {

    }

    ~Sandbox() {

    }
};

Solar::Application* Solar::CreateApplication() {
    return new Sandbox();
}
