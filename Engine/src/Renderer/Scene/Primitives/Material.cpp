#include "../../../kitapch.h"
#include "Material.h"

#include "../../../Core/Engine.h"

namespace Kita {
    Material::Material() {
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader(ShaderManager::DEFAULT_VERTEX,
                                                                                   ShaderManager::DEFAULT_FRAGMENT);
        m_phongUniformBuffer->createBuffer(sizeof(PhongProperties), &m_phongProperties);
    }

    Material::Material(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
        m_phongUniformBuffer->createBuffer(sizeof(PhongProperties), &m_phongProperties);
    }

    const std::shared_ptr<Shader>& Material::getShader() {
        return m_shader;
    }

    void Material::setShader(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
    }

    const std::vector<std::shared_ptr<Texture>>& Material::getTextures() {
        return m_textures;
    }

    void Material::addTexture(const std::shared_ptr<Texture>& texture) {
        m_textures.push_back(texture);
    }

    void Material::replaceTexture(std::shared_ptr<Texture>& texture, const size_t position) {
        m_textures.at(position).swap(texture);
    }

    const PhongProperties& Material::getPhongProperties() const {
        return m_phongProperties;
    }

    void Material::setPhongProperties(const PhongProperties& phongProperties) {
        m_phongProperties = phongProperties;
        m_phongUniformBuffer->bind(1);
        m_phongUniformBuffer->update(sizeof(m_phongProperties), &m_phongProperties);
    }

    const std::shared_ptr<UniformBuffer>& Material::getPhongUniformBuffer() {
        return m_phongUniformBuffer;
    }
} // Kita
