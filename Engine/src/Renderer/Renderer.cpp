#include "../kitapch.h"
#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_shaderManager = std::make_unique<ShaderManager>();
        m_rendererAPI->init(*this);
    }

    RenderingAPI Renderer::getAPI() {
        return m_api;
    }

    ShaderManager& Renderer::getShaderManager() const {
        return *m_shaderManager;
    }

    TextureManager& Renderer::getTextureManager() const {
        return *m_textureManager;
    }

    RendererAPI& Renderer::getRendererAPI() const {
        return *m_rendererAPI;
    }

} // Kita