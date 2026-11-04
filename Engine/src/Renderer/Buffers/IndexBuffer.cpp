#include "IndexBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLIndexBuffer.h"

namespace Kita {
    std::unique_ptr<IndexBuffer> IndexBuffer::createPtr(std::vector<unsigned int> indices) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLIndexBuffer>(indices);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int IndexBuffer::getIBO() const {
        return m_ibo;
    }

    const std::vector<unsigned int>& IndexBuffer::getIndices() {
        return m_indices;
    }

    IndexBuffer::IndexBuffer(std::vector<unsigned int> indices) : m_indices(std::move(indices)) {
    }
}
