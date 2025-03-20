#include "IndexBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.h"

namespace Kita {
    unsigned int IndexBuffer::getEBO() const {
        return m_ebo;
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case GraphicsAPI::OPENGL:
                return std::make_shared<GLIndexBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create ElementBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
}
