#pragma once

#include "Types/RenderingAPI.h"
#include "../Core/DllTemplate.h"
#include <memory>

#include "RendererAPI.h"

namespace Kita {
    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(RenderingAPI api);
        static RenderingAPI getAPI();

        RendererAPI& getRendererAPI() const;

    private:
        static inline RenderingAPI m_api = RenderingAPI::NONE;
        std::unique_ptr<RendererAPI> m_rendererAPI;
    };
} // Kita