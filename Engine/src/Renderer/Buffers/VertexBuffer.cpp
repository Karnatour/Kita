#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers//GLVertexBuffer.h"

namespace Kita {
    std::unique_ptr<VertexBuffer> VertexBuffer::createPtr(VertexBufferData bufferData) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLVertexBuffer>(std::move(bufferData));
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int VertexBuffer::getVBO() const {
        return m_vbo;
    }

    const VertexBufferData& VertexBuffer::getBufferData() const {
        return m_bufferData;
    }

    VertexBuffer::VertexBuffer(VertexBufferData bufferData) : m_bufferData(std::move(bufferData)) {
    }
} // Kita
