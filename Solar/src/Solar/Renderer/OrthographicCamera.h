#pragma once

namespace Solar {
    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera();

        void SetProjection(float left, float right, float bottom, float top);

        const glm::vec3& GetPosition() const;
        void SetPosition(const glm::vec3& position);

        float GetRotation() const;
        void SetRotation(float rotation);

        const glm::mat4& GetProjectionMatrix() const;
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetViewProjectionMatrix() const;
    private:
        void RecalculateViewMatrix();
    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;
        float m_Rotation;
    };
}
