#include "VertexArray.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.h"

namespace Kita {
    unsigned int VertexArray::getVAO() const {
        return m_vao;
    }

    std::shared_ptr<VertexArray> VertexArray::createPtr() {
        switch (Renderer::getAPI()) {
            case GraphicsAPI::OPENGL:
                return std::make_shared<GLVertexArray>();
            default:
                KITA_ENGINE_ERROR("Trying to create VertexArray while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
} // Kita
