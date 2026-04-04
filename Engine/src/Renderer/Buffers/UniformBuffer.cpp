#include "../../kitapch.h"
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
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
        }
    }
} // Kita