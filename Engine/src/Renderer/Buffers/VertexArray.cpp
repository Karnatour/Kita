#include "VertexArray.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLVertexArray.h"

namespace Kita {
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

    unsigned int VertexArray::getVAO() const {
        return m_vao;
    }
} // Kita
