#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_shaderManager = std::make_unique<ShaderManager>();
    }

    RenderingAPI Renderer::getAPI() {
        return m_api;
    }

    const ShaderManager& Renderer::getShaderManager() const {
        return *m_shaderManager;
    }

    RendererAPI& Renderer::getRendererAPI() const {
        return *m_rendererAPI;
    }

} // Kita