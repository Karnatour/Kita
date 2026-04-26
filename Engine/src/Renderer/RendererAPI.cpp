#include "../kitapch.h"
#include "RendererAPI.h"

#include "../Core/Engine.h"
#include "GraphicsAPI/OpenGL/GLRendererAPI.h"

namespace Kita {
    std::unique_ptr<RendererAPI> RendererAPI::createPtr(const RenderingAPI api) {
        switch (api) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLRendererAPI>();
            default:
                KITA_ENGINE_ERROR("Trying to create RendererAPI while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    void RendererAPI::init() {
        setupDebug();
    }
} // Kita
