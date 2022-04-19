#pragma once

#include "Solar/Renderer/Framebuffer.h"

namespace Solar {
    class OpenGLFramebuffer :public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        virtual void Bind() override;
        virtual void Unbind() override;

        virtual uint32_t GetColorAttachmentRendererID() const override;

        virtual const FramebufferSpecification& GetSpecification() const override;

        void Invalidate();
    private:
        uint32_t m_RendererID;
        uint32_t m_ColorAttachment;
        uint32_t m_DepthAttachment;
        FramebufferSpecification m_Specification;
    };
}
