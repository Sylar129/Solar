#include <Solar.h>
#include <Solar/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Solar {
    class SolarEditor : public Application {
    public:
        SolarEditor() : Application("Solar Editor") {
            // PushLayer(new ExampleLayer());
            PushLayer(new EditorLayer());
        }

        ~SolarEditor() {

        }
    };

    Application* CreateApplication() {
        return new SolarEditor();
    }
}

