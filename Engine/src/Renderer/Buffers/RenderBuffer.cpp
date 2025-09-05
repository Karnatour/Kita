#include "../../kitapch.h"
#include "RenderBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLRenderBuffer.h"

namespace Kita {
    unsigned int RenderBuffer::getRBO() const {
        return m_rbo;
    }

    std::shared_ptr<RenderBuffer> RenderBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLRenderBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create RenderBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    std::pair<int, int> RenderBuffer::getResolution() const {
        return m_resolution;
    }
} // Kita