#pragma once
#include "../../Core/DllTemplate.h"
#include "Shader.h"
#include <unordered_map>
#include <memory>
#include <filesystem>

namespace Kita {
    //TODO Fix material/shader lifetime
    class KITAENGINE_API ShaderManager {
    public:
        static inline const std::filesystem::path SHADER_PREFIX = "../assets/shaders/";
        static inline const std::filesystem::path DEFAULT_VERTEX = "DefaultVertex.glsl";
        static inline const std::filesystem::path DEFAULT_FRAGMENT = "DefaultFragment.glsl";

        ShaderManager();
        void addShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        std::shared_ptr<Shader> getShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) const;
        const std::unordered_map<std::filesystem::path, std::shared_ptr<Shader>>& getShaderMap() const;

    private:
        std::unordered_map<std::filesystem::path, std::shared_ptr<Shader>> m_shaderMap;
    };
} // Kita
