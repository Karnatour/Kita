#pragma once
#include "../src/Core/DllTemplate.h"
#include <memory>
#include <string>

namespace Kita {
    class KITAENGINE_API Texture {
    public:
        virtual ~Texture() = default;
        virtual bool createTexture(const std::string& texturePath) = 0;
        virtual void bind(unsigned int position) = 0;
        static std::shared_ptr<Texture> createPtr();
        unsigned int getTexture() const;
        int getWidth() const;
        int getHeight() const;
        int getChannels() const;
    protected:
        unsigned int m_texture = 0;
        int m_width = 0;
        int m_height = 0;
        int m_channels = 0;;
    };
} // Kita
