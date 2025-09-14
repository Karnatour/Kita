#include "../../kitapch.h"
#include "FrameBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLFrameBuffer.h"

namespace Kita {
    unsigned int FrameBuffer::getFBO() const {
        return m_fbo;
    }

    std::shared_ptr<FrameBuffer> FrameBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLFrameBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create FrameBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    std::pair<int, int> FrameBuffer::getResolution() const {
        return m_resolution;
    }

    std::shared_ptr<Texture> FrameBuffer::getDepthTexture() const {
        return m_depthTexture;
    }
} // Kita