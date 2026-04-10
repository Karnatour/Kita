#include "../../kitapch.h"
#include "IndexBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.h"

namespace Kita {
    unsigned int IndexBuffer::getIBO() const {
        return m_ibo;
    }

    std::unique_ptr<IndexBuffer> IndexBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLIndexBuffer>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }

    const std::vector<unsigned int>& IndexBuffer::getIndices() {
        return m_indices;
    }
}
