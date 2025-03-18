#pragma once
#include "Scene.h"

namespace Kita {
    class Renderer {
    public:
        explicit Renderer(GraphicsAPI api);
        static GraphicsAPI getAPI();

    private:
        std::unique_ptr<Scene> m_scene;
        static inline GraphicsAPI m_api = GraphicsAPI::NONE;
    };
} // Kita
