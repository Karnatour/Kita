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
            default:
                KITA_ENGINE_ERROR("Trying to create VertexBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    size_t VertexBuffer::getVerticiesCount() const {
        return m_verticiesCount;
    }

    std::vector<Vertex>& VertexBuffer::getVertices() {
        return m_vertices;
    }
} // Kita
