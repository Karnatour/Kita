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

    private:
        void compileGLShader(const std::string& path, const GLuint& shader);
        void checkShaderCompilation(GLuint shader, const std::string& shaderPath);

        void compileGLProgram(GLuint vertexShader, GLuint fragmentShader);
        void checkProgramLinkage(const std::string& vertexPath, const std::string& fragmentPath);
    };
} // Kita
