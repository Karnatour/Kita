#pragma once
#include "../Core/DllTemplate.h"
#include "Structs/RenderingAPI.h"

namespace Kita {

class KITAENGINE_API RendererAPI {
public:
    virtual ~RendererAPI() = default;
    virtual void setViewport(int width, int height) = 0;
    static std::unique_ptr<RendererAPI> createPtr(RenderingAPI api);
private:
};

} // Kita
