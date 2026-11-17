#include "../Assets/Texture.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/GraphicsAPI/OpenGL/GLTextures/GLTexture.h"

namespace Kita {
    std::unique_ptr<Texture> Texture::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLTexture>();
            default:
                KITA_ENGINE_ERROR("Trying to create Texture while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    unsigned int Texture::getTexture() const {
        return m_texture;
    }

    int Texture::getWidth() const {
        return m_width;
    }

    int Texture::getHeight() const {
        return m_height;
    }

    int Texture::getChannels() const {
        return m_channels;
    }

    bool Texture::isHighPrecision() const {
        return m_highPrecision;
    }

    std::span<const std::filesystem::path> Texture::getPaths() const {
        return m_paths;
    }

    Texture::TextureType Texture::getType() const {
        return m_textureType;
    }

    std::pair<int, int> Texture::getResolution() const{
        return std::make_pair(m_width, m_height);
    }
} // Kita
