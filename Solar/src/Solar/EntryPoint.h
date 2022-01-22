#ifndef _SOLAR_ENTRYPOINT_H_
#define _SOLAR_ENTRYPOINT_H_

#ifdef SOLAR_PLATFORM_WINDOWS

extern Solar::Application* Solar::CreateApplication();

int main(int argc, char** argv) {
    auto app = Solar::CreateApplication();
    app->Run();
    delete app;
}


#endif // SOLAR_PLATFORM_WINDOWS

#endif // _SOLAR_ENTRYPOINT_H_
