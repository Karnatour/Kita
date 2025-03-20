#pragma once
#include "Scene.h"
#include "Structs/GraphicsAPI.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Renderer {
    public:
        explicit Renderer(GraphicsAPI api);
        static GraphicsAPI getAPI();

    private:
        std::unique_ptr<Scene> m_scene = nullptr;
        static inline GraphicsAPI m_api = GraphicsAPI::NONE;
    };
} // Kita
