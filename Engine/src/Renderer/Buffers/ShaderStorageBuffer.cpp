#include "ShaderStorageBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLShaderStorageBuffer.h"

namespace Kita {
    std::unique_ptr<ShaderStorageBuffer> ShaderStorageBuffer::createPtr(size_t size, const void* data) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLShaderStorageBuffer>(size, data);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int ShaderStorageBuffer::getSSBO() const {
        return m_ssbo;
    }

    ShaderStorageBuffer::ShaderStorageBuffer(const size_t size) : m_size(size) {
    }
} // Kita
