#pragma once
#include <glad/glad.h>

#include "../../../Shaders/Shader.h"

namespace Kita {
    class GLShader final : public Shader {
    public:
        ~GLShader() override;
        void bind() override;
        void compileShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) override;

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
        void compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath);
        void compileGLProgram(GLuint vertexShader, GLuint fragmentShader);
    };
} // Kita
