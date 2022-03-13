#include <Solar.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Solar::Layer {
public:
    ExampleLayer()
      : Layer("Example"),
        m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
        m_CameraPosition(0.0f) {
        /// <summary>
        /// Triangle Renderer
        /// </summary>
        m_VertexArray = Solar::VertexArray::Create();

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        Solar::Ref<Solar::VertexBuffer> vertexBuffer;
        vertexBuffer = Solar::VertexBuffer::Create(vertices, sizeof(vertices));

        vertexBuffer->SetLayout({
            { Solar::ShaderDataType::Float3, "a_Position" },
            { Solar::ShaderDataType::Float4, "a_Color" }
                                });
        m_VertexArray->AddVertexBuffer(vertexBuffer);

        Solar::Ref<Solar::IndexBuffer> indexBuffer;
        uint32_t indices[3] = { 0,1,2 };
        indexBuffer = Solar::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(indexBuffer);

        /// <summary>
        /// Triangle Shader
        /// </summary>
        std::string vertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;

            void main() {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string fragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;

            void main() {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

        m_Shader = Solar::Shader::Create(vertexSrc, fragmentSrc);


        /// <summary>
        /// Square Renderer
        /// </summary>
        m_SquareVA = Solar::VertexArray::Create();

        float squareVertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };
        Solar::Ref<Solar::VertexBuffer> squreVB;
        squreVB = Solar::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

        squreVB->SetLayout({
            { Solar::ShaderDataType::Float3, "a_Position" },
            { Solar::ShaderDataType::Float2, "a_TexCoord" },
        });
        m_SquareVA->AddVertexBuffer(squreVB);

        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Solar::Ref<Solar::IndexBuffer> squreIB;
        squreIB = Solar::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        m_SquareVA->SetIndexBuffer(squreIB);

        /// <summary>
        /// Square Shader
        /// </summary>
        std::string squareVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;

            void main() {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string squareFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec3 v_Position;

            uniform vec3 u_Color;

            void main() {
                color = vec4(u_Color, 1.0);
            }
        )";

        m_SquareShader = Solar::Shader::Create(squareVertexSrc, squareFragmentSrc);

        /// <summary>
        /// Texture Shader
        /// </summary>
        std::string textureShaderVertexSrc = R"(
            #version 330 core

            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec2 a_TexCoord;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec2 v_TexCoord;

            void main() {
                v_TexCoord = a_TexCoord;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

        std::string textureShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 color;

            in vec2 v_TexCoord;

            uniform sampler2D u_Texture;

            void main() {
                color = texture(u_Texture, v_TexCoord);
            }
        )";

        m_TextureShader = Solar::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc);
        m_Texture = Solar::Texture2D::Create("assets/textures/Board.png");

        m_LogoTexture = Solar::Texture2D::Create("assets/textures/Logo.png");

        std::dynamic_pointer_cast<Solar::OpenGLShader>(m_TextureShader)->Bind();
        std::dynamic_pointer_cast<Solar::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Solar::TimeStep& ts) override {
        // SOLAR_INFO("ExampleLayer::Update");
        // SOLAR_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        if (Solar::Input::IsKeyPressed(SOLAR_KEY_LEFT)) {
            m_CameraPosition.x -= m_CameraSpeed * ts;
        }
        if (Solar::Input::IsKeyPressed(SOLAR_KEY_RIGHT)) {
            m_CameraPosition.x += m_CameraSpeed * ts;
        }
        if (Solar::Input::IsKeyPressed(SOLAR_KEY_UP)) {
            m_CameraPosition.y += m_CameraSpeed * ts;
        }
        if (Solar::Input::IsKeyPressed(SOLAR_KEY_DOWN)) {
            m_CameraPosition.y -= m_CameraSpeed * ts;
        }

        if (Solar::Input::IsKeyPressed(SOLAR_KEY_Q)) {
            m_CameraRotation += m_CameraRotationSpeed * ts;
        }
        if (Solar::Input::IsKeyPressed(SOLAR_KEY_E)) {
            m_CameraRotation -= m_CameraRotationSpeed * ts;
        }

        /// <summary>
        /// Renderer
        /// </summary>
        Solar::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Solar::RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Solar::Renderer::BeginScene(m_Camera);

        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        std::dynamic_pointer_cast<Solar::OpenGLShader>(m_SquareShader)->Bind();
        std::dynamic_pointer_cast<Solar::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Solar::Renderer::Submit(m_SquareShader, m_SquareVA, transform);
            }
        }

        m_Texture->Bind();
        Solar::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        m_LogoTexture->Bind();
        Solar::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

        // Solar::Renderer::Submit(m_Shader, m_VertexArray);

        Solar::Renderer::EndScene();

    }

    void OnImGuiRender() override {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Solar::Event& event) override {
        // SOLAR_TRACE("ExampleLayer: {0}", event);
        Solar::EventDispatcher dispatcher(event);
        // dispatcher.Dispatch<Solar::KeyPressdEvent>(SOLAR_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
    }

    bool OnKeyPressedEvent(Solar::KeyPressdEvent& event) {
        if (event.GetKeyCode() == SOLAR_KEY_LEFT) {
            m_CameraPosition.x -= m_CameraSpeed;
        }
        if (event.GetKeyCode() == SOLAR_KEY_RIGHT) {
            m_CameraPosition.x += m_CameraSpeed;
        }
        if (event.GetKeyCode() == SOLAR_KEY_UP) {
            m_CameraPosition.y += m_CameraSpeed;
        }
        if (event.GetKeyCode() == SOLAR_KEY_DOWN) {
            m_CameraPosition.y -= m_CameraSpeed;
        }

        return false;
    }

private:
    Solar::Ref<Solar::Shader> m_Shader;
    Solar::Ref<Solar::VertexArray> m_VertexArray;

    Solar::Ref<Solar::Shader> m_SquareShader, m_TextureShader;
    Solar::Ref<Solar::VertexArray> m_SquareVA;

    Solar::Ref<Solar::Texture2D> m_Texture, m_LogoTexture;

    Solar::OrthographicCamera m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraSpeed = 0.5f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 0.5f;

    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Solar::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox() {

    }
};

Solar::Application* Solar::CreateApplication() {
    return new Sandbox();
}
