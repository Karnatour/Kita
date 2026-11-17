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
        if (!isExtensionSupported(Extension::SHADER_VIEWPORT_LAYER_ARRAY)) {
            KITA_ENGINE_ERROR("GL_ARB_shader_viewport_layer_array is not supported, the engine will exit");
            Engine::getEngine()->stop();
        }
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }
} // Kita
