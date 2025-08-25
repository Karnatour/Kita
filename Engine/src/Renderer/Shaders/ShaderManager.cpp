#include "../../kitapch.h"
#include "ShaderManager.h"

namespace Kita {
    ShaderManager::ShaderManager() {
        addShader(DEFAULT_VERTEX, DEFAULT_FRAGMENT);
    }

    void ShaderManager::addShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        auto [iterator,inserted] = m_shaderMap.try_emplace(vertexPath.string() + ":" + fragmentPath.string(), Shader::createPtr());

        if (inserted) {
            iterator->second->compileShader(vertexPath, fragmentPath);
            KITA_ENGINE_DEBUG("Added shader to ShaderManager, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.string(), fragmentPath.string());
        }
        else {
            if (vertexPath != DEFAULT_VERTEX && fragmentPath != DEFAULT_FRAGMENT) {
                KITA_ENGINE_WARN("Shader is already included in ShaderManager, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.string(), fragmentPath.string());
            }
        }
    }

    std::shared_ptr<Shader> ShaderManager::getShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) const {
        auto iterator = m_shaderMap.find(vertexPath.string() + ":" + fragmentPath.string());
        if (iterator != m_shaderMap.end()) {
            return iterator->second;
        }
        KITA_ENGINE_ERROR("Shader not found in ShaderManager returning nullptr, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.string(), fragmentPath.string());
        return nullptr;
    }

    const std::unordered_map<std::filesystem::path, std::shared_ptr<Shader>>& ShaderManager::getShaderMap() const {
        return m_shaderMap;
    }
} // Kita
