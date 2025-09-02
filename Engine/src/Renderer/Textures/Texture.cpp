#include "../../kitapch.h"
#include "Texture.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLTexture.h"

namespace Kita {
    std::shared_ptr<Texture> Texture::createPtr() {
        switch (Renderer::getAPI()) {
        case RenderingAPI::OPENGL:
            return std::make_shared<GLTexture>();
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

    const std::filesystem::path& Texture::getPath() const {
        return m_path;
    }

    Texture::TextureType Texture::getType() const {
        return m_textureType;
    }
} // Kita