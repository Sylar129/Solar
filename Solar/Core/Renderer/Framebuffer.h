#pragma once

#include <vector>

#include "Core/Base/Sundry.h"

namespace Solar {

enum class FramebufferTextureFormat {
    None = 0,

    // Color
    RGBA8,
    RED_INTEGER,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
    FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
    // TODO: filtering/wrap

    FramebufferTextureSpecification() = default;
    FramebufferTextureSpecification(FramebufferTextureFormat format)
        : TextureFormat(format)
    {
    }
};

struct FramebufferAttachmentSpecification {
    std::vector<FramebufferTextureSpecification> Attachments;

    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(
        std::initializer_list<FramebufferTextureSpecification> attachments)
        : Attachments(attachments)
    {
    }
};

struct FramebufferSpecification {
    uint32_t Width, Height;
    FramebufferAttachmentSpecification Attachments;
    uint32_t Samples = 1;

    bool SwapChainTarget = false;
};

class Framebuffer {
public:
    virtual ~Framebuffer() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;
    virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

    virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

    virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

    // virtual FramebufferSpecification& GetSpecification() = 0;
    virtual const FramebufferSpecification& GetSpecification() const = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

} // namespace Solar
