#include "../../kitapch.h"
#include "VertexArray.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.h"

namespace Kita {
    const std::unique_ptr<VertexBuffer>& VertexArray::getVBOobj() const {
        return m_vbo;
    }

    const std::unique_ptr<IndexBuffer>& VertexArray::getIBOobj() const {
        return m_ibo;
    }

    unsigned int VertexArray::getVAO() const {
        return m_vao;
    }

    std::unique_ptr<VertexArray> VertexArray::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLVertexArray>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }
} // Kita
