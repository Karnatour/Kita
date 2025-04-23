#pragma once
#include <glad/glad.h>

#include "../../../Shaders/Shader.h"

namespace Kita {
    class GLShader final : public Shader {
    public:
        ~GLShader() override;
        void bind() override;
        void compileShader(const std::string& vertexPath, const std::string& fragmentPath) override;

        void setUniformBool(const std::string& location, bool value) override;
        void setUniformFloat(const std::string& location, float value) override;
        void seUniformtInt(const std::string& location, int value) override;
        void setVec2(const std::string& location, const glm::vec2& value) override;
        void setVec3(const std::string& location, const glm::vec3& value) override;
        void setVec4(const std::string& location, const glm::vec4& value) override;
        void setMat2(const std::string& location, const glm::mat2& value) override;
        void setMat3(const std::string& location, const glm::mat3& value) override;
        void setMat4(const std::string& location, const glm::mat4& value) override;

    private:
        void compileGLShader(const GLuint& shader, const std::string& shaderPath);
        void compileGLProgram(GLuint vertexShader, GLuint fragmentShader);
    };
} // Kita
