#pragma once

#include "Solar/Renderer/OrthographicCamera.h"
#include "Solar/Core/Timestep.h"

#include "Solar/Events/ApplicationEvent.h"
#include "Solar/Events/MouseEvent.h"

namespace Solar {
    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(TimeStep ts);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float m_AspectRatio;
        float m_ZoomLevel;
        OrthographicCamera m_Camera;

        glm::vec3 m_CameraPosition;
        float m_CameraTranslationSpeed;

        bool m_Rotation;
        float m_CameraRotation;     // radians
        float m_CameraRotationSpeed;
    };
}
