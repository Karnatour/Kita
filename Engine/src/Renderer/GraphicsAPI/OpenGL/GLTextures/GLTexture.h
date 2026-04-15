#pragma once
#include "../../../../Assets/Texture.h"

namespace Kita {
    class GLTexture final : public Texture {
    public:
        ~GLTexture() override;
        std::expected<void, TextureError> createTexture(const std::filesystem::path& texturePath, TextureType textureType, std::optional<std::pair<int, int>> resolution) override;
        void createBufferTypeTexture(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision) override;
        void bind(unsigned int position) override;
        void destroy() override;

    private:
        std::expected<void, TextureError> createSkyboxTexture();
        std::expected<void, TextureError> createCubemapTexture(const std::pair<int, int>& resolution);
    };
} // Kita
