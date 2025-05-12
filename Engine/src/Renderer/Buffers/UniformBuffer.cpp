#include "UniformBuffer.h"
#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLUniformBuffer.h"

namespace Kita {
    unsigned int UniformBuffer::getUBO() const {
        return m_ubo;
    }

    std::shared_ptr<UniformBuffer> UniformBuffer::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLUniformBuffer>();
            default:
                KITA_ENGINE_ERROR("Trying to create UniformBuffer while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }
} // Kita