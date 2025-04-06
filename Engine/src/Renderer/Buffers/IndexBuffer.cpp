#include "IndexBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.h"

namespace Kita {
    unsigned int IndexBuffer::getIBO() const {
        return m_ibo;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLIndexBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create ElementBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    size_t IndexBuffer::getIndicesCount() const {
        return m_indicesCount;
    }
}
