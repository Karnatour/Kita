#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers//GLVertexBuffer.h"

namespace Kita {
    unsigned int VertexBuffer::getVBO() const {
        return m_vbo;
    }

    std::unique_ptr<VertexBuffer> VertexBuffer::create() {
        switch (Renderer::getAPI()) {
            case GraphicsAPI::OPENGL:
                return std::make_unique<GLVertexBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create VertexBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
} // Kita
