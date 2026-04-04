#include "../../kitapch.h"
#include "RenderBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLRenderBuffer.h"

namespace Kita {
    unsigned int RenderBuffer::getRBO() const {
        return m_rbo;
    }

    std::unique_ptr<RenderBuffer> RenderBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLRenderBuffer>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }

    std::pair<int, int> RenderBuffer::getResolution() const {
        return m_resolution;
    }
} // Kita