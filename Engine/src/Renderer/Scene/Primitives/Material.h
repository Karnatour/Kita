#pragma once

#include "../../../Core/DllTemplate.h"
#include "../../Shaders/Shader.h"
#include "../../Structs/PhongProperties.h"
#include "../../Textures/Texture.h"

namespace Kita {
    class KITAENGINE_API Material {
    public:
        Material();
        explicit Material(const std::shared_ptr<Shader>& shader);

        const std::shared_ptr<Shader>& getShader() const;
        void setShader(const std::shared_ptr<Shader>& shader);

        const std::shared_ptr<Texture>& getDiffuseTexture() const;
        void setDiffuseTexture(const std::shared_ptr<Texture>& diffuseTexture);

        const std::shared_ptr<Texture>& getSpecularTexture() const;
        void setSpecularTexture(const std::shared_ptr<Texture>& specularTexture);

        const std::shared_ptr<Texture>& getNormalTexture() const;
        void setNormalTexture(const std::shared_ptr<Texture>& normalTexture);

        const PhongProperties& getPhongProperties() const;
        void setPhongProperties(const PhongProperties& phongProperties);

    private:
        std::shared_ptr<Shader> m_shader;
        std::shared_ptr<Texture> m_diffuseTexture;
        std::shared_ptr<Texture> m_specularTexture;
        std::shared_ptr<Texture> m_normalTexture;
        PhongProperties m_phongProperties;
    };
} // Kita
