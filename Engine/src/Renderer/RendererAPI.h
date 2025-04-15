#pragma once
#include "../Core/DllTemplate.h"
#include "Structs/RenderingAPI.h"
#include <memory>

#include "Mesh.h"

namespace Kita {

class KITAENGINE_API RendererAPI {
public:
    virtual ~RendererAPI() = default;
    virtual void setViewport(int width, int height) = 0;
    virtual void render(Mesh& mesh) = 0;
    virtual void clearColor(float red, float green, float blue, float alpha) = 0;
    static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
    virtual void setupDebug() = 0;
};

} // Kita
