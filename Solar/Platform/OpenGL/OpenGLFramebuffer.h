// Copyright (c) 2024 Sylar129

#pragma once

#include "Core/Renderer/Framebuffer.h"

namespace Solar {

class OpenGLFramebuffer : public Framebuffer {
 public:
  explicit OpenGLFramebuffer(const FramebufferSpecification& spec);
  virtual ~OpenGLFramebuffer();

  virtual void Bind() override;
  virtual void Unbind() override;

  virtual void Resize(uint32_t width, uint32_t height) override;
  virtual int ReadPixel(uint32_t attachment_index, int x, int y) override;

  virtual void ClearAttachment(uint32_t attachment_index, int value) override;

  virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const override;

  virtual const FramebufferSpecification& GetSpecification() const override;

  void Invalidate();

 private:
  uint32_t renderer_id_;
  FramebufferSpecification specification_;

  std::vector<FramebufferTextureSpecification> color_attachment_specifications_;
  FramebufferTextureSpecification depth_attachment_specifications_;

  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_;
};

}  // namespace Solar
