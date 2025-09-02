#include "../../kitapch.h"
#include "ShaderStorageBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLShaderStorageBuffer.h"

namespace Kita {
    unsigned int ShaderStorageBuffer::getSSBO() const {
        return m_ssbo;
    }

    std::shared_ptr<ShaderStorageBuffer> ShaderStorageBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLShaderStorageBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create UniformBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
} // Kita