#pragma once
#include "../../RendererAPI.h"

namespace Kita {

class GLRendererAPI final : public RendererAPI{
public:
    void setViewport(int width, int height) override;
    void render(Mesh& mesh) override;
    void clearColor(float red, float green, float blue, float alpha) override;
};

} // Kita
