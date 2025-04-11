#include "../../kitapch.h"
#include "ShaderManager.h"

namespace Kita {
    ShaderManager::ShaderManager() {
        addShader("../assets/shaders/DefaultVertexColor.glsl","../assets/shaders/DefaultFragmentColor.glsl");
        addShader("../assets/shaders/DefaultVertexTexture.glsl","../assets/shaders/DefaultFragmentTexture.glsl");
    }

    void ShaderManager::addShader(const std::string& vertexPath, const std::string& fragmentPath) {
        auto [iterator,inserted] = m_shaderMap.try_emplace(vertexPath + ":" + fragmentPath, Shader::createPtr());

        if (inserted) {
            iterator->second->compileShader(vertexPath, fragmentPath);
            KITA_ENGINE_DEBUG("Added shader to ShaderManager, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.c_str(), fragmentPath.c_str());
        }
        else {
            KITA_ENGINE_WARN("Shader is already included in ShaderManager, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.c_str(), fragmentPath.c_str());
        }
    }

    std::shared_ptr<Shader> ShaderManager::getShader(const std::string& vertexPath, const std::string& fragmentPath) const {
        auto iterator = m_shaderMap.find(vertexPath + ":" + fragmentPath);
        if (iterator != m_shaderMap.end()) {
            return iterator->second;
        }
        KITA_ENGINE_ERROR("Shader not found in ShaderManager returning nullptr, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.c_str(), fragmentPath.c_str());
        return nullptr;
    }

    const std::unordered_map<std::string, std::shared_ptr<Shader>>& ShaderManager::getShaderMap() const {
        return m_shaderMap;
    }
} // Kita
