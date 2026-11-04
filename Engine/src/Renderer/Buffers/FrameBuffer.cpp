#include "FrameBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLFrameBuffer.h"

namespace Kita {
    std::unique_ptr<FrameBuffer> FrameBuffer::createPtr(const std::optional<std::pair<int, int>>& resolution, std::initializer_list<AttachInfo> attachments, bool highPrecision, int layers) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLFrameBuffer>(resolution, attachments, highPrecision, layers);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int FrameBuffer::getFBO() const {
        return m_fbo;
    }

    std::pair<int, int> FrameBuffer::getResolution() const {
        return m_resolution;
    }

    Texture* FrameBuffer::getColorTexture() const {
        return m_colorTexture.get();
    }

    Texture* FrameBuffer::getDepthTexture() const {
        return m_depthTexture.get();
    }

    RenderBuffer* FrameBuffer::getColorRenderBuffer() const {
        return m_colorRenderBuffer.get();
    }

    RenderBuffer* FrameBuffer::getDepthRenderBuffer() const {
        return m_depthRenderBuffer.get();
    }

    FrameBuffer::FrameBuffer(const std::optional<std::pair<int, int>>& resolution, const std::initializer_list<AttachInfo> attachments, const bool highPrecision, const int layers)
        : m_resolution(resolution.value_or(std::make_pair(-1,-1))), m_attachments(attachments), m_highPrecision(highPrecision), m_layers(layers) {
    }
} // Kita
