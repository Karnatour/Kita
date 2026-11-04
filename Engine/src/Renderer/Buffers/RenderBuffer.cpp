#include "RenderBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLRenderBuffer.h"

namespace Kita {
    std::unique_ptr<RenderBuffer> RenderBuffer::createPtr(const std::pair<int, int>& resolution, BufferType bufferType) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLRenderBuffer>(resolution, bufferType);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int RenderBuffer::getRBO() const {
        return m_rbo;
    }

    std::pair<int, int> RenderBuffer::getResolution() const {
        return m_resolution;
    }

    RenderBuffer::RenderBuffer(const std::pair<int, int>& resolution, const BufferType bufferType) : m_resolution(resolution), m_bufferType(bufferType) {
    }
} // Kita
