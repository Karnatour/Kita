#include "../../kitapch.h"
#include "VertexArray.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.h"

namespace Kita {
    std::shared_ptr<VertexBuffer> VertexArray::getVBOobj() const {
        return m_vbo;
    }

    std::shared_ptr<IndexBuffer> VertexArray::getIBOobj() const {
        return m_ibo;
    }

    unsigned int VertexArray::getVAO() const {
        return m_vao;
    }

    std::shared_ptr<VertexArray> VertexArray::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLVertexArray>();
            default:
                KITA_ENGINE_ERROR("Trying to create VertexArray while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
} // Kita
