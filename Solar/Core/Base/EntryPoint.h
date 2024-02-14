// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Debug/Instrumentor.h"

#ifdef SOLAR_PLATFORM_WINDOWS

extern solar::Application* solar::CreateApplication();

int main(int argc, char** argv) {
  solar::Log::Init();
  SOLAR_CORE_WARN("Initialized Log!");
  SOLAR_INFO("Hello! Solar Engine.");

  SOLAR_PROFILE_BEGIN_SESSION("Startup", "SolarProfile-Startup.json");
  auto app = solar::CreateApplication();
  SOLAR_PROFILE_END_SESSION();

  SOLAR_PROFILE_BEGIN_SESSION("Runtime", "SolarProfile-Runtime.json");
  app->Run();
  SOLAR_PROFILE_END_SESSION();

  SOLAR_PROFILE_BEGIN_SESSION("Shutdown", "SolarProfile-Shutdown.json");
  delete app;
  SOLAR_PROFILE_END_SESSION();
}

#endif  // SOLAR_PLATFORM_WINDOWS
