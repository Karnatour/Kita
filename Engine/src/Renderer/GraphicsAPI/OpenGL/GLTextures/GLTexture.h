#pragma once
#include <glad/glad.h>

#include "../../../../Assets/Texture.h"

namespace Kita {
    class GLTexture final : public Texture {
    public:
        ~GLTexture() override;
        std::expected<void, TextureError> createTexture(std::vector<std::filesystem::path> texturePaths, TextureType textureType, std::optional<std::pair<int, int>> resolution) override;
        void createBufferTypeTexture(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision) override;
        void createBufferTypeTextureArray(std::pair<int, int> resolution, BufferType bufferType, bool highPrecision, int layersCount) override;
        void bind(unsigned int position) override;
        void generateMipmaps() override;
        void destroy() override;

    private:
        std::expected<void, TextureError> setTexturesInformation();
        GLenum getInternalFormat(bool useSRGB) const;
        GLenum getTextureFormat() const;
        std::expected<void, TextureError> loadDDSTexture();
        std::expected<void, TextureError> createSkyboxTexture();
        std::expected<void, TextureError> createCubemapTexture(const std::pair<int, int>& resolution);
    };
} // Kita
