#pragma once

#include "Solar/Renderer/Framebuffer.h"

namespace Solar {
    class OpenGLFramebuffer :public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

        virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

        virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

        virtual const FramebufferSpecification& GetSpecification() const override;

        void Invalidate();
    private:
        uint32_t m_RendererID;
        FramebufferSpecification m_Specification;

        std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
        FramebufferTextureSpecification m_DepthAttachmentSpecifications;

        std::vector<uint32_t> m_ColorAttachments;
        uint32_t m_DepthAttachment;
    };
}
