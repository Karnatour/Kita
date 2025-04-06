#pragma once
#include "../../RendererAPI.h"

namespace Kita {

class GLRendererAPI final : public RendererAPI{
public:
    void setViewport(int width, int height) override;
};

} // Kita
