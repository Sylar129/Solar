#pragma once

#ifdef SOLAR_PLATFORM_WINDOWS

extern Solar::Application* Solar::CreateApplication();

int main(int argc, char** argv)
{
    Solar::Log::Init();
    SOLAR_CORE_WARN("Initialized Log!");
    SOLAR_INFO("Hello! Solar Engine.");

    SOLAR_PROFILE_BEGIN_SESSION("Startup", "SolarProfile-Startup.json");
    auto app = Solar::CreateApplication();
    SOLAR_PROFILE_END_SESSION();

    SOLAR_PROFILE_BEGIN_SESSION("Runtime", "SolarProfile-Runtime.json");
    app->Run();
    SOLAR_PROFILE_END_SESSION();

    SOLAR_PROFILE_BEGIN_SESSION("Shutdown", "SolarProfile-Shutdown.json");
    delete app;
    SOLAR_PROFILE_END_SESSION();
}

#endif // SOLAR_PLATFORM_WINDOWS
