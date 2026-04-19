#include "../../kitapch.h"
#include "ShaderStorageBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLShaderStorageBuffer.h"

namespace Kita {
    unsigned int ShaderStorageBuffer::getSSBO() const {
        return m_ssbo;
    }

    std::unique_ptr<ShaderStorageBuffer> ShaderStorageBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLShaderStorageBuffer>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }
} // Kita