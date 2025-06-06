#include "GLTexture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Kita {
    GLTexture::~GLTexture() {
        glDeleteTextures(1,&m_texture);
    }

    bool GLTexture::createTexture(const std::string& texturePath) {
        stbi_set_flip_vertically_on_load(true);

        unsigned char* image = stbi_load(texturePath.c_str(), &m_width, &m_height, &m_channels, 0);
        if (image) {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);

            //TODO: Support parameter option
            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            switch (m_channels) {
                case 3:
                    glTextureStorage2D(m_texture, 1, GL_RGB8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, image);
                    break;
                case 4:
                    glTextureStorage2D(m_texture, 1, GL_RGBA8, m_width, m_height);
                    glTextureSubImage2D(m_texture, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, image);
                    break;
                default:
                    KITA_ENGINE_ERROR("Unsupported number of channels for texture, {}", texturePath.c_str());
                    return false;
                    break;
            }

            glGenerateTextureMipmap(m_texture);
            stbi_image_free(image);
        }
        else {
            KITA_ENGINE_ERROR("Unable to load texture, {}", texturePath.c_str());
            return false;
        }
        return true;
    }

    void GLTexture::bind(const unsigned int position) {
        glBindTextureUnit(position, m_texture);
    }
} // Kita
