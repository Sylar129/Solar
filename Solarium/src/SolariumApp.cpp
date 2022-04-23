#include <Solar.h>
#include <Solar/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Solar {
    class Solarium : public Application {
    public:
        Solarium() : Application("Solarium") {
            // PushLayer(new ExampleLayer());
            PushLayer(new EditorLayer());
        }

        ~Solarium() {

        }
    };

    Application* CreateApplication() {
        return new Solarium();
    }
}

