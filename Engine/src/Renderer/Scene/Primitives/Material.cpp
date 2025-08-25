#include "../../../kitapch.h"
#include "Material.h"

#include "../../../Core/Engine.h"

namespace Kita {
    Material::Material() {
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader(ShaderManager::DEFAULT_VERTEX,
                                                                                   ShaderManager::DEFAULT_FRAGMENT);
    }

    Material::Material(const std::filesystem::path& texturePath) {
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader(ShaderManager::DEFAULT_VERTEX,
                                                                                   ShaderManager::DEFAULT_FRAGMENT);
        auto& texManager = Engine::getEngine()->getRenderer().getTextureManager();
        texManager.addTexture(texturePath);
        addTexture(texManager.getTexture(texturePath));
    }

    std::shared_ptr<Shader>& Material::getShader() {
        return m_shader;
    }


    void Material::setShader(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
    }

    std::vector<std::shared_ptr<Texture>>& Material::getTextures() {
        return m_textures;
    }

    void Material::addTexture(const std::shared_ptr<Texture>& texture) {
        m_textures.push_back(texture);
    }

    PhongProperties& Material::getPhongProperties() {
        return m_phongProperties;
    }
} // Kita
