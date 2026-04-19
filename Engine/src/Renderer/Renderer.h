#pragma once

#include "Types/RenderingAPI.h"
#include "../Core/DllTemplate.h"
#include <memory>

#include "RendererAPI.h"

namespace Kita {
    class Mesh;
    class Shader;
    class Texture;

    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(RenderingAPI api);
        static RenderingAPI getAPI();

        RendererAPI& getRendererAPI() const;
        std::pair<float, float> getViewport() const;
        void renderMesh(const Mesh& mesh, const Shader& shader, const Transformation& transformation, std::span<Texture* const> textures);

    private:
        std::pair<float, float> m_viewport = std::make_pair(1600.0f, 900.0f);
        static inline RenderingAPI m_api = RenderingAPI::NONE;
        std::unique_ptr<RendererAPI> m_rendererAPI;
    };
} // Kita