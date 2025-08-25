#pragma once
#include "../../Textures/Texture.h"

namespace Kita {

class GLTexture final : public Texture{
public:
    ~GLTexture() override;
    bool createTexture(const std::filesystem::path& texturePath) override;
    void bind(unsigned int position) override;
};

} // Kita
