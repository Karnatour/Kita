#pragma once
#include "Scene.h"
#include "Structs/RenderingAPI.h"
#include "../Core/DllTemplate.h"
#include "Shaders/ShaderManager.h"
#include <memory>

#include "RendererAPI.h"

namespace Kita {
    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(RenderingAPI api);
        static RenderingAPI getAPI();

        const ShaderManager& getShaderManager() const;
        RendererAPI& getRendererAPI();

    private:
        static inline RenderingAPI m_api = RenderingAPI::NONE;
        std::unique_ptr<RendererAPI> m_rendererAPI;
        std::unique_ptr<ShaderManager> m_shaderManager;
        std::unique_ptr<Scene> m_scene;
    };
} // Kita