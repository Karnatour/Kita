#pragma once

#include <filesystem>

#include "../../../Core/DllTemplate.h"
#include "../../Shaders/Shader.h"
#include "../../Structs/PhongProperties.h"
#include "../../Textures/Texture.h"
#include <vector>

#include "../../Buffers/UniformBuffer.h"

namespace Kita {
    class KITAENGINE_API Material {
    public:
        Material();
        explicit Material(const std::shared_ptr<Shader>& shader);

        const std::shared_ptr<Shader>& getShader();
        void setShader(const std::shared_ptr<Shader>& shader);

        const std::vector<std::shared_ptr<Texture>>& getTextures();
        void addTexture(const std::shared_ptr<Texture>& texture);
        void replaceTexture(std::shared_ptr<Texture>& texture, size_t position);

        const PhongProperties& getPhongProperties() const;
        void setPhongProperties(const PhongProperties& phongProperties);

        const std::shared_ptr<UniformBuffer>& getPhongUniformBuffer();
    private:
        std::shared_ptr<Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<Texture>> m_textures;
        PhongProperties m_phongProperties;
        //TODO One UBO as array for all materials, do something about materials that don't need phong properties
        std::shared_ptr<UniformBuffer> m_phongUniformBuffer = UniformBuffer::createPtr();
    };
} // Kita
