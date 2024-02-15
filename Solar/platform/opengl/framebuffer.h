// Copyright (c) 2024 Sylar129

#pragma once

#include <vector>

#include "core/renderer/framebuffer.h"

namespace solar {

class OpenGLFramebuffer : public Framebuffer {
 public:
  explicit OpenGLFramebuffer(const FramebufferSpecification& spec);
  ~OpenGLFramebuffer() override;

  void Bind() override;
  void Unbind() override;

  void Resize(float width, float height) override;
  int ReadPixel(uint32_t attachment_index, int x, int y) override;

  void ClearAttachment(uint32_t attachment_index, int value) override;

  uint32_t GetColorAttachmentRendererID(uint32_t index) const override;

  const FramebufferSpecification& GetSpecification() const override;

  void Invalidate();

 private:
  uint32_t renderer_id_;
  FramebufferSpecification specification_;

  std::vector<FramebufferTextureSpecification> color_attachment_specifications_;
  FramebufferTextureSpecification depth_attachment_specifications_;

  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_;
};

}  // namespace solar
