#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers//GLVertexBuffer.h"

namespace Kita {
    std::unique_ptr<VertexBuffer> VertexBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLVertexBuffer>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }

    unsigned int VertexBuffer::getVBO() const {
        return m_vbo;
    }

    const std::vector<VertexProperties>& VertexBuffer::getVertices() {
        return m_vertices;
    }
} // Kita
