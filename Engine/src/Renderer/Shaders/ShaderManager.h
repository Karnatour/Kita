#pragma once
#include "../../Core/DllTemplate.h"
#include "Shader.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace Kita {
    class KITAENGINE_API ShaderManager {
    public:
        ShaderManager();
        void addShader(const std::string& vertexPath, const std::string& fragmentPath);
        std::shared_ptr<Shader> getShader(const std::string& vertexPath, const std::string& fragmentPath) const;
        const std::unordered_map<std::string, std::shared_ptr<Shader>>& getShaderMap() const;

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderMap;
    };
} // Kita
