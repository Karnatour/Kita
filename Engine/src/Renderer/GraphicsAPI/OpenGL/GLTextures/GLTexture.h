#pragma once
#include "../../../Textures/Texture.h"

namespace Kita {

class GLTexture final : public Texture{
public:
    ~GLTexture() override;
    bool createTexture2D(const std::filesystem::path& texturePath, const TextureType& textureType) override;
    bool createSkyboxTexture2D(const std::filesystem::path& texturePath) override;
    void createCubemapTexture(const std::pair<int, int>& resolution) override;
    void createBufferTypeTexture(const std::pair<int, int>& resolution, const BufferType& bufferType, bool highPrecision) override;
    void bind(unsigned int position) override;
    void destroy() override;
};

} // Kita
