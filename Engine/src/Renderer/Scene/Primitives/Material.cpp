#include "../../../kitapch.h"
#include "Material.h"

#include "../../../Core/Engine.h"

namespace Kita {
    Material::Material() {
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader(ShaderManager::DEFAULT_VERTEX, ShaderManager::DEFAULT_FRAGMENT);
    }

    Material::Material(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
    }

    const std::shared_ptr<Shader>& Material::getShader() const {
        return m_shader;
    }

    void Material::setShader(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
    }

    const std::shared_ptr<Texture>& Material::getDiffuseTexture() const {
        return m_diffuseTexture;
    }

    void Material::setDiffuseTexture(const std::shared_ptr<Texture>& diffuseTexture)  {
        m_diffuseTexture = diffuseTexture;
    }

    const std::shared_ptr<Texture>& Material::getSpecularTexture() const {
        return m_specularTexture;
    }

    void Material::setSpecularTexture(const std::shared_ptr<Texture>& specularTexture) {
        m_specularTexture = specularTexture;
    }

    const std::shared_ptr<Texture>& Material::getNormalTexture() const {
        return m_normalTexture;
    }

    void Material::setNormalTexture(const std::shared_ptr<Texture>& normalTexture) {
        m_normalTexture = normalTexture;
    }

    const PhongProperties& Material::getPhongProperties() const {
        return m_phongProperties;
    }

    void Material::setPhongProperties(const PhongProperties& phongProperties) {
        m_phongProperties = phongProperties;
    }
} // Kita
