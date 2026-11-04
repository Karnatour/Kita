#include "DrawCommandBuffer.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLBuffers/GLDrawCommandBuffer.h"

namespace Kita {
    std::unique_ptr<DrawCommandBuffer> DrawCommandBuffer::createPtr(const std::vector<DrawArraysIndirectCommandProperties>& arraysCommands) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLDrawCommandBuffer>(arraysCommands);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    std::unique_ptr<DrawCommandBuffer> DrawCommandBuffer::createPtr(const std::vector<DrawElementsIndirectCommandProperties>& elementsCommands) {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLDrawCommandBuffer>(elementsCommands);
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    unsigned int DrawCommandBuffer::getDCB() const {
        return m_dcb;
    }

    DrawCommandBuffer::Type DrawCommandBuffer::getType() const {
        return m_type;
    }

    DrawCommandBuffer::DrawCommandBuffer(const Type type) : m_type(type) {
    }
} // Kita
