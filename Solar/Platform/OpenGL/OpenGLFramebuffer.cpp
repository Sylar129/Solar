// Copyright (c) 2024 Sylar129

#include "Platform/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>

#include "Core/Base/Log.h"

namespace Solar {

namespace {
constexpr uint32_t kMaxFramebufferSize = 8192;

constexpr GLenum TextureTarget(bool multisampled) {
  return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

void CreateTextures(bool multisampled, uint32_t* out_id, uint32_t count) {
  glCreateTextures(TextureTarget(multisampled), count, out_id);
}

void BindTexture(bool multisampled, uint32_t id) {
  glBindTexture(TextureTarget(multisampled), id);
}

void AttachColorTexture(uint32_t id, int samples, GLenum internal_format,
                        GLenum format, uint32_t width, uint32_t height,
                        int index) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internal_format,
                            width, height, GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         TextureTarget(multisampled), id, 0);
}

void AttachDepthTexture(uint32_t id, int samples, GLenum format,
                        GLenum attachment_type, uint32_t width,
                        uint32_t height) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                            height, GL_FALSE);
  } else {
    glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type,
                         TextureTarget(multisampled), id, 0);
}

constexpr bool IsDepthFormat(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::kDEPTH24STENCIL8:
      return true;
  }
  return false;
}

constexpr GLenum SolarFBTextureFormatToGL(FramebufferTextureFormat format) {
  switch (format) {
    case FramebufferTextureFormat::kRGBA8:
      return GL_RGBA8;
    case FramebufferTextureFormat::kRED_INTEGER:
      return GL_RED_INTEGER;
  }
  SOLAR_CORE_ASSERT(false, "Unknown format!");
  return 0;
}

// TODO
// constexpr GLenum GLDataType(FramebufferTextureFormat format) {
//    switch (format) {
//        case FramebufferTextureFormat::RGBA8:
//            return GL_FLOAT;
//        case FramebufferTextureFormat::RED_INTEGER:
//            return GL_RED_INTEGER;
//    }
//    SOLAR_CORE_ASSERT(false, "Unknown format!");
//    return 0;
//}

}  // namespace

OpenGLFramebuffer::OpenGLFramebuffer(
    const FramebufferSpecification& specification)
    : renderer_id_(0), specification_(specification), depth_attachment_(0) {
  for (const auto& spec : specification_.attachments.attachments) {
    if (!IsDepthFormat(spec.texture_format)) {
      color_attachment_specifications_.emplace_back(spec);
    } else {
      depth_attachment_specifications_ = spec;
    }
  }

  Invalidate();
}

OpenGLFramebuffer::~OpenGLFramebuffer() {
  glDeleteFramebuffers(1, &renderer_id_);
  glDeleteTextures(color_attachments_.size(), color_attachments_.data());
  glDeleteTextures(1, &depth_attachment_);
}

void OpenGLFramebuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);
  glViewport(0, 0, specification_.width, specification_.height);
}

void OpenGLFramebuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
  if (width == 0 || height == 0 || width > kMaxFramebufferSize ||
      height > kMaxFramebufferSize) {
    SOLAR_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width,
                    height);
    return;
  }

  specification_.width = width;
  specification_.height = height;

  Invalidate();
}

int OpenGLFramebuffer::ReadPixel(uint32_t attachment_index, int x, int y) {
  SOLAR_CORE_ASSERT(attachment_index < color_attachments_.size(),
                    "Index out of range!");
  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
  int pixel_data = 0;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
  return pixel_data;
}

void OpenGLFramebuffer::ClearAttachment(uint32_t attachment_index, int value) {
  SOLAR_CORE_ASSERT(attachment_index < color_attachments_.size(),
                    "Index out of range!");

  auto& spec = color_attachment_specifications_[attachment_index];

  glClearTexImage(color_attachments_[attachment_index], 0,
                  SolarFBTextureFormatToGL(spec.texture_format), GL_INT,
                  &value);
}

uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index) const {
  SOLAR_CORE_ASSERT(index < color_attachments_.size(),
                    "Index shoule be less than size!");
  return color_attachments_[index];
}

const FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const {
  return specification_;
}

void OpenGLFramebuffer::Invalidate() {
  if (renderer_id_) {
    glDeleteFramebuffers(1, &renderer_id_);
    glDeleteTextures(color_attachments_.size(), color_attachments_.data());
    glDeleteTextures(1, &depth_attachment_);

    color_attachments_.clear();
    depth_attachment_ = 0;
  }

  glCreateFramebuffers(1, &renderer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, renderer_id_);

  bool multisample = specification_.samples > 1;

  // Attachments
  if (color_attachment_specifications_.size()) {
    color_attachments_.resize(color_attachment_specifications_.size());
    CreateTextures(multisample, color_attachments_.data(),
                   color_attachments_.size());

    for (auto i{0}; i < color_attachment_specifications_.size(); i++) {
      BindTexture(multisample, color_attachments_[i]);
      switch (color_attachment_specifications_[i].texture_format) {
        case FramebufferTextureFormat::kRGBA8:
          AttachColorTexture(color_attachments_[i], specification_.samples,
                             GL_RGBA8, GL_RGBA, specification_.width,
                             specification_.height, i);
          break;
        case FramebufferTextureFormat::kRED_INTEGER:
          AttachColorTexture(color_attachments_[i], specification_.samples,
                             GL_R32I, GL_RED_INTEGER, specification_.width,
                             specification_.height, i);
          break;
      }
    }
  }

  if (depth_attachment_specifications_.texture_format !=
      FramebufferTextureFormat::kNone) {
    CreateTextures(multisample, &depth_attachment_, 1);
    BindTexture(multisample, depth_attachment_);
    switch (depth_attachment_specifications_.texture_format) {
      case FramebufferTextureFormat::kDEPTH24STENCIL8:
        AttachDepthTexture(depth_attachment_, specification_.samples,
                           GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT,
                           specification_.width, specification_.height);
        break;
    }
  }

  if (color_attachments_.size() > 1) {
    SOLAR_CORE_ASSERT(color_attachments_.size() < 4,
                      "The size of ColorAttachments should less than 4!");
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(color_attachments_.size(), buffers);
  } else if (color_attachments_.empty()) {
    // Only depth-pass
    glDrawBuffer(GL_NONE);
  }

  SOLAR_CORE_ASSERT(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "Framebuffer is incomplete!");

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace Solar
