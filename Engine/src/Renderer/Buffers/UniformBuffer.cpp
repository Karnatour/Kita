#include "UniformBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLUniformBuffer.h"

namespace Kita {
    std::unique_ptr<UniformBuffer> UniformBuffer::createPtr(size_t size, const void* data) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLUniformBuffer>(size, data);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int UniformBuffer::getUBO() const {
        return m_ubo;
    }

    UniformBuffer::UniformBuffer(const size_t size) : m_size(size) {
    }
} // Kita
