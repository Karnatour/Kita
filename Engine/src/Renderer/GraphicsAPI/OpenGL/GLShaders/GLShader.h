#pragma once
#include <glad/glad.h>

#include "../../../../Assets/Shader.h"

namespace Kita {
    class GLShader final : public Shader {
    public:
        ~GLShader() override;
        void bind() override;
        std::expected<void, ShaderError> createShader(std::span<const ShaderInfo> shaders) override;

        void setUniformBool(const std::string& location, bool value) override;
        void setUniformFloat(const std::string& location, float value) override;
        void setUniformInt(const std::string& location, int value) override;
        void setUniformVec2(const std::string& location, const glm::vec2& value) override;
        void setUniformVec3(const std::string& location, const glm::vec3& value) override;
        void setUniformVec4(const std::string& location, const glm::vec4& value) override;
        void setUniformMat2(const std::string& location, const glm::mat2& value) override;
        void setUniformMat3(const std::string& location, const glm::mat3& value) override;
        void setUniformMat4(const std::string& location, const glm::mat4& value) override;

    private:
        GLenum getGLShaderType(ShaderType type);
        void releaseCompiledShaders(const std::vector<unsigned int>& glShaders);
        std::expected<void, ShaderError> compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath, const std::vector<ShaderDefine>& defines);
        std::expected<void, ShaderError> linkGLProgram(const std::vector<GLuint>& glShaders);
    };
} // Kita
