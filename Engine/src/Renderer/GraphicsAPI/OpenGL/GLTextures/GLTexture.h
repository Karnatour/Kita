#pragma once
#include "../../../Textures/Texture.h"

namespace Kita {

class GLTexture final : public Texture{
public:
    ~GLTexture() override;
    bool createTexture2D(const std::filesystem::path& texturePath, const TextureType& textureType) override;
    bool createSkyboxTexture2D(const std::filesystem::path& texturePath) override;
    void createCubemapTexture(const std::pair<int, int>& resolution) override;
    void createBufferTypeTexture(const BufferType& bufferType, const std::pair<int, int>& resolution) override;
    void bind(unsigned int position) override;
};

} // Kita
