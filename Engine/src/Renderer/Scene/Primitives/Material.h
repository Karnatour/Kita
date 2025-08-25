#pragma once

#include <filesystem>

#include "../../../Core/DllTemplate.h"
#include "../../Shaders/Shader.h"
#include "../../Structs/PhongProperties.h"
#include "../../Textures/Texture.h"
#include <vector>

namespace Kita {
    class KITAENGINE_API Material {
    public:
        Material();
        explicit Material(const std::filesystem::path& texturePath);

        std::shared_ptr<Shader>& getShader();
        void setShader(const std::shared_ptr<Shader>& shader);

        std::vector<std::shared_ptr<Texture>>& getTextures();
        void addTexture(const std::shared_ptr<Texture>& texture);

        PhongProperties& getPhongProperties();

    private:
        std::shared_ptr<Shader> m_shader = nullptr;
        std::vector<std::shared_ptr<Texture>> m_textures;
        PhongProperties m_phongProperties;
    };
} // Kita
