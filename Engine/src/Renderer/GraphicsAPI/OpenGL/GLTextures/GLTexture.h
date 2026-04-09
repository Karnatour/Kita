#pragma once
#include "../../../../Assets/Texture.h"

namespace Kita {

class GLTexture final : public Texture {
public:
    ~GLTexture() override;
    std::expected<void,TextureError> createTexture(const std::filesystem::path& texturePath, TextureType textureType, std::optional<std::pair<int, int>> resolution) override;
    bool createSkyboxTexture2D(const std::filesystem::path& texturePath) override;
    void createCubemapTexture(const std::pair<int, int>& resolution) override;
    void createBufferTypeTexture(const std::pair<int, int>& resolution, const BufferType& bufferType, bool highPrecision) override;
    void bind(unsigned int position) override;
    void destroy() override;
};

} // Kita
