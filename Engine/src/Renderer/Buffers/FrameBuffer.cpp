#include "../../kitapch.h"
#include "FrameBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLFrameBuffer.h"

namespace Kita {
    unsigned int FrameBuffer::getFBO() const {
        return m_fbo;
    }

    std::unique_ptr<FrameBuffer> FrameBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLFrameBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create FrameBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    std::pair<int, int> FrameBuffer::getResolution() const {
        return m_resolution;
    }

    std::shared_ptr<Texture> FrameBuffer::getColorTexture() const {
        return m_colorTexture;
    }

    std::shared_ptr<Texture> FrameBuffer::getDepthTexture() const {
        return m_depthTexture;
    }

    const RenderBuffer* FrameBuffer::getColorRenderBuffer() const {
        return m_colorRenderBuffer.get();
    }

    const RenderBuffer* FrameBuffer::getDepthRenderBuffer() const {
        return m_depthRenderBuffer.get();
    }

} // Kita