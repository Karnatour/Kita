#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const GraphicsAPI api) {
        m_api = api;
        m_shaderManager = std::make_unique<ShaderManager>();
    }

    GraphicsAPI Renderer::getAPI() {
        return m_api;
    }

    const ShaderManager& Renderer::getShaderManager() const {
        return *m_shaderManager;
    }
} // Kita