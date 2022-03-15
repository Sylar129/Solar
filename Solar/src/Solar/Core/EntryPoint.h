#pragma once

#ifdef SOLAR_PLATFORM_WINDOWS

extern Solar::Application* Solar::CreateApplication();

int main(int argc, char** argv) {
    Solar::Log::Init();
    SOLAR_CORE_WARN("Initialized Log!");
    SOLAR_INFO("Hello! Solar Engine.");

    auto app = Solar::CreateApplication();
    app->Run();
    delete app;
}

#endif // SOLAR_PLATFORM_WINDOWS
