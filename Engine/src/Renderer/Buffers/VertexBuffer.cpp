#include "../../kitapch.h"
#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers//GLVertexBuffer.h"

namespace Kita {
    unsigned int VertexBuffer::getVBO() const {
        return m_vbo;
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLVertexBuffer>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }

    size_t VertexBuffer::getVerticesCount() const {
        return m_verticesCount;
    }

    std::vector<Vertex>& VertexBuffer::getVertices() {
        return m_vertices;
    }
} // Kita
