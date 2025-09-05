#pragma once
#include "../Core/DllTemplate.h"
#include "Structs/RenderingAPI.h"
#include <memory>

namespace Kita {
    class Entity;

    class KITAENGINE_API RendererAPI {
    public:
        virtual ~RendererAPI() = default;
        virtual void setViewport(int width, int height, bool rewriteStoredPair) = 0;
        virtual void restoreViewport() = 0;
        virtual void render(const std::shared_ptr<Entity>& entity) = 0;
        virtual void clearColor(float red, float green, float blue, float alpha) = 0;
        static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
        virtual void setupDebug() = 0;
        virtual void enableDepthTest() = 0;
    };

} // Kita
