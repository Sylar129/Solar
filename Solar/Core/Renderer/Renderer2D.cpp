#include "solpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "Core/Renderer/RenderCommand.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Solar {

struct QuadVertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;

    // Editor-only
    int EntityID;
};

struct Renderer2DData {
    static const uint32_t MaxQuads = 10000;
    static const uint32_t MaxVertices = MaxQuads * 4;
    static const uint32_t MaxIndices = MaxQuads * 6;
    static const uint32_t MaxTextureSlots = 32; // TODO: Render capability

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t QuadIndexCount = 0;
    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = white texture

    glm::vec4 QuadVertexPositions[4];

    Renderer2D::Statistics Stats;
};

static Renderer2DData s_Data;

void Renderer2D::Init()
{
    SOLAR_PROFILE_FUNCTION();

    s_Data.QuadVertexArray = Solar::VertexArray::Create();

    s_Data.QuadVertexBuffer =
        VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
    s_Data.QuadVertexBuffer->SetLayout(
        {{ShaderDataType::Float3, "a_Position"},
         {ShaderDataType::Float4, "a_Color"},
         {ShaderDataType::Float2, "a_TexCoord"},
         {ShaderDataType::Float, "a_TexIndex"},
         {ShaderDataType::Float, "a_TilingFactor"},
         {ShaderDataType::Int, "a_EntityID"}});
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

    uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (int i = 0; i < s_Data.MaxIndices; i += 6) {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }
    Ref<IndexBuffer> quadIB =
        IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
    s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
    delete[] quadIndices;

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; ++i) {
        samplers[i] = i;
    }

    s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetIntArray("u_Texture", samplers,
                                      s_Data.MaxTextureSlots);

    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
}

void Renderer2D::ShutDown()
{
    SOLAR_PROFILE_FUNCTION();

    delete[] s_Data.QuadVertexBufferBase;
}

void Renderer2D::BeginScene(const Camera& camera, const glm::mat4 transform)
{
    SOLAR_PROFILE_FUNCTION();

    glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

    StartBatch();
}

void Renderer2D::BeginScene(const EditorCamera& camera)
{
    SOLAR_PROFILE_FUNCTION();

    glm::mat4 viewProj = camera.GetViewProjection();

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

    StartBatch();
}

void Renderer2D::BeginScene(const OrthographicCamera& camera)
{
    SOLAR_PROFILE_FUNCTION();

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection",
                                  camera.GetViewProjectionMatrix());

    StartBatch();
}

void Renderer2D::EndScene()
{
    SOLAR_PROFILE_FUNCTION();

    Flush();
}

void Renderer2D::Flush()
{
    if (0 == s_Data.QuadIndexCount)
        return;

    uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr -
                                   (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    // Bind textures
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; ++i) {
        s_Data.TextureSlots[i]->Bind(i);
    }

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const glm::vec4& color)
{
    DrawQuad({position.x, position.y, 0.0f}, size, color);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const glm::vec4& color)
{
    SOLAR_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    DrawQuad(transform, color);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          const glm::vec4& tintColor)
{
    DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor,
             tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          const glm::vec4& tintColor)
{
    SOLAR_PROFILE_FUNCTION();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    DrawQuad(transform, texture, tilingFactor, tintColor);
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size,
                          const Ref<SubTexture2D>& subTexture,
                          float tilingFactor, const glm::vec4& tintColor)
{
    DrawQuad({position.x, position.y, 0.0f}, size, subTexture, tilingFactor,
             tintColor);
}

void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size,
                          const Ref<SubTexture2D>& subTexture,
                          float tilingFactor, const glm::vec4& tintColor)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    const glm::vec2* textureCoords = subTexture->GetTexCoords();
    const Ref<Texture2D> texture = subTexture->GetTexture();

    float textureIndex = 0.0f;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
            NextBatch();
        }

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
                          int entityID)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    constexpr float textureIndex = 0.0f; // White texture
    constexpr float tilingFactor = 1.0f;
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr->EntityID = entityID;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawQuad(const glm::mat4& transform,
                          const Ref<Texture2D>& texture, float tilingFactor,
                          const glm::vec4& tintColor, int entityID)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    constexpr glm::vec2 textureCoords[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    float textureIndex = 0.0f;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
            NextBatch();
        }

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr->EntityID = entityID;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color)
{
    DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, color);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const glm::vec4& color)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    const float textureIndex = 0.0f; // White texture
    const float tilingFactor = 1.0f;
    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                                      {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture,
                                float tilingFactor, const glm::vec4& tintColor)
{
    DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, texture,
                   tilingFactor, tintColor);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const Ref<Texture2D>& texture,
                                float tilingFactor, const glm::vec4& tintColor)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    constexpr glm::vec2 textureCoords[] = {
        {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

    float textureIndex = 0.0f;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
            NextBatch();
        }

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                                      {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawRotateQuad(const glm::vec2& position,
                                const glm::vec2& size, float rotation,
                                const Ref<SubTexture2D>& subTexture,
                                float tilingFactor, const glm::vec4& tintColor)
{
    DrawRotateQuad({position.x, position.y, 0.0f}, size, rotation, subTexture,
                   tilingFactor, tintColor);
}

void Renderer2D::DrawRotateQuad(const glm::vec3& position,
                                const glm::vec2& size, float rotation,
                                const Ref<SubTexture2D>& subTexture,
                                float tilingFactor, const glm::vec4& tintColor)
{
    SOLAR_PROFILE_FUNCTION();

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices) {
        NextBatch();
    }

    constexpr size_t quadVertexCount = 4;
    constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    const glm::vec2* textureCoords = subTexture->GetTexCoords();
    const Ref<Texture2D> texture = subTexture->GetTexture();

    float textureIndex = 0.0f;

    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i].get() == *texture.get()) {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f) {
        if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots) {
            NextBatch();
        }

        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
                                      {0.0f, 0.0f, 1.0f}) *
                          glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

    for (uint32_t i = 0; i < quadVertexCount; ++i) {
        s_Data.QuadVertexBufferPtr->Position =
            transform * s_Data.QuadVertexPositions[i];
        s_Data.QuadVertexBufferPtr->Color = color;
        s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
        s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
        s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;

    s_Data.Stats.QuadCount++;
}

void Renderer2D::DrawSprite(const glm::mat4& transform,
                            SpriteRendererComponent& src, int entityID)
{
    DrawQuad(transform, src.Color, entityID);
}

void Renderer2D::ResetStats()
{
    memset(&s_Data.Stats, 0, sizeof(s_Data.Stats));
}

Renderer2D::Statistics Renderer2D::GetStats() { return s_Data.Stats; }

void Renderer2D::StartBatch()
{
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
    s_Data.QuadIndexCount = 0;
    s_Data.TextureSlotIndex = 1;
}

void Renderer2D::NextBatch()
{
    Flush();
    StartBatch();
}

} // namespace Solar