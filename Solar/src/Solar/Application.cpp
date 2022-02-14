#include "Application.h"

#include "Solar/Events/ApplicationEvent.h"
#include "Solar/Log.h"

namespace Solar {
    Application::Application() {

    }

    Application::~Application() {

    }

    void Application::Run() {
        WindowResizeEvent e(1280, 720);
        SOLAR_TRACE(e);


        while (true);
    }
}
