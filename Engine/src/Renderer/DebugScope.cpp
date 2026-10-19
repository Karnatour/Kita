#include "DebugScope.h"

#include <memory>

#include "Renderer.h"
#include "GraphicsAPI/OpenGL/GLDebugScope.h"

namespace Kita {
    std::unique_ptr<DebugScope> DebugScope::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLDebugScope>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }
} // Kita
