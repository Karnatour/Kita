#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_rendererAPI->setupDebug();
        m_shaderManager = std::make_unique<ShaderManager>();
        m_textureManager = std::make_unique<TextureManager>();
    }

    RenderingAPI Renderer::getAPI() {
        return m_api;
    }

    const ShaderManager& Renderer::getShaderManager() const {
        return *m_shaderManager;
    }

    const TextureManager& Renderer::getTextureManager() const {
        return *m_textureManager;
    }

    RendererAPI& Renderer::getRendererAPI() const {
        return *m_rendererAPI;
    }

} // Kita