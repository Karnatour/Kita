#include "Renderer.h"

namespace Kita {
    Renderer::Renderer(const GraphicsAPI api) {
        m_api = api;
    }

    GraphicsAPI Renderer::getAPI() {
        return m_api;
    }
} // Kita