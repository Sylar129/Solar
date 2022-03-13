#include "solpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Solar {
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
      : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
        m_ViewMatrix(1.0f),
        m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix),
        m_Position({0.0f, 0.0f, 0.0f}),
        m_Rotation(0.0f) {

    }

    OrthographicCamera::~OrthographicCamera() {

    }

    const glm::vec3& OrthographicCamera::GetPosition() const {
        return m_Position;
    }

    void OrthographicCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }

    float OrthographicCamera::GetRotation() const {
        return m_Rotation;
    }

    void OrthographicCamera::SetRotation(float rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    const glm::mat4& OrthographicCamera::GetProjectionMatrix() const {
        return m_ProjectionMatrix;
    }

    const glm::mat4& OrthographicCamera::GetViewMatrix() const {
        return m_ViewMatrix;
    }

    const glm::mat4& OrthographicCamera::GetViewProjectionMatrix() const {
        return m_ViewProjectionMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
            * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}