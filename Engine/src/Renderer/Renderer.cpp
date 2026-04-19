#include "../kitapch.h"
#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const RenderingAPI api) {
        m_api = api;
        m_rendererAPI = RendererAPI::createPtr(api);
        m_shaderManager = std::make_unique<ShaderManager>();
        m_rendererAPI->init(*this);
        m_shaderManager->init();
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

    std::pair<float, float> Renderer::getViewport() const {
        return m_viewport;
    }

    void Renderer::renderMesh(const Mesh& mesh, const Shader& shader, const Transformation& transformation, std::span<Texture* const> textures) {
    }

} // Kita