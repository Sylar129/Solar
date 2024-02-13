// Copyright (c) 2024 Sylar129

#pragma once

#include <vector>

#include "Core/Base/Sundry.h"

namespace Solar {

enum class FramebufferTextureFormat {
  kNone = 0,

  // Color
  kRGBA8,
  kRED_INTEGER,

  // Depth/stencil
  kDEPTH24STENCIL8,

  // Defaults
  kDepth = kDEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
  FramebufferTextureFormat texture_format = FramebufferTextureFormat::kNone;
  // TODO: filtering/wrap

  FramebufferTextureSpecification() = default;
  FramebufferTextureSpecification(FramebufferTextureFormat format)
      : texture_format(format) {}
};

struct FramebufferAttachmentSpecification {
  std::vector<FramebufferTextureSpecification> attachments;

  FramebufferAttachmentSpecification() = default;
  FramebufferAttachmentSpecification(
      std::initializer_list<FramebufferTextureSpecification> attachments)
      : attachments(attachments) {}
};

struct FramebufferSpecification {
  uint32_t width;
  uint32_t height;
  FramebufferAttachmentSpecification attachments;
  uint32_t samples = 1;

  bool swap_chain_target = false;
};

class Framebuffer {
 public:
  virtual ~Framebuffer() = default;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;

  virtual void Resize(uint32_t width, uint32_t height) = 0;
  virtual int ReadPixel(uint32_t attachment_index, int x, int y) = 0;

  virtual void ClearAttachment(uint32_t attachment_index, int value) = 0;

  virtual uint32_t GetColorAttachmentRendererID(uint32_t index) const = 0;

  // virtual FramebufferSpecification& GetSpecification() = 0;
  virtual const FramebufferSpecification& GetSpecification() const = 0;

  static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
};

}  // namespace Solar
