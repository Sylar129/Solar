#include "solpch.h"
#include "OrthographicCameraController.h"

#include "Solar/Core/Input.h"
#include "Solar/Core/KeyCodes.h"

namespace Solar {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
      : m_AspectRatio(aspectRatio),
        m_ZoomLevel(1.0f),
        m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
        m_CameraPosition(0.0f, 0.0f, 0.0f),
        m_CameraTranslationSpeed(1.0f),
        m_Rotation(rotation),
        m_CameraRotation(0.0f),
        m_CameraRotationSpeed(1.0f) {

    }

    void OrthographicCameraController::OnUpdate(TimeStep ts) {
        SOLAR_PROFILE_FUNCTION();

        if (Input::IsKeyPressed(KeyCode::A)) {
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(KeyCode::D)) {
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(KeyCode::W)) {
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        }
        if (Input::IsKeyPressed(KeyCode::S)) {
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        }

        if (m_Rotation) {
            if (Input::IsKeyPressed(KeyCode::Q)) {
                m_CameraRotation += m_CameraRotationSpeed * ts;
            }
            if (Input::IsKeyPressed(KeyCode::E)) {
                m_CameraRotation -= m_CameraRotationSpeed * ts;
            }

            if (m_CameraRotation > 180.0f) {
                m_CameraRotation -= 360.0f;
            } else if (m_CameraRotation <= -180.0f) {
                m_CameraRotation += 360.0f;
            }

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_Camera.SetPosition(m_CameraPosition);
        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e) {
        SOLAR_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(SOLAR_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(SOLAR_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
        SOLAR_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.1f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
        SOLAR_PROFILE_FUNCTION();

        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}
