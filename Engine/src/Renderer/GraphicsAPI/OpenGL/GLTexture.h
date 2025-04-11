#pragma once
#include "../../Textures/Texture.h"

namespace Kita {

class GLTexture final : public Texture{
public:
    bool createTexture(const std::string& texturePath) override;
    void bind(unsigned int position) override;
};

} // Kita
