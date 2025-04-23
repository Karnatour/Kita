#include "GLShader.h"

namespace Kita {
    void GLShader::bind() {
        glUseProgram(m_program);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_program);
    }

    void GLShader::compileShader(const std::string& vertexPath, const std::string& fragmentPath) {
        KITA_ENGINE_DEBUG("Starting compilation of shaders, [VERTEX]->{}, [FRAGMENT]->{}", vertexPath.c_str(), fragmentPath.c_str());

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        compileGLShader(vertexShader, vertexPath);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        compileGLShader(fragmentShader, fragmentPath);

        compileGLProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void GLShader::compileGLShader(const GLuint& shader, const std::string& shaderPath) {
        const std::string& shaderSource = FileReader::readFile(shaderPath);
        const char* sourcePtr = shaderSource.data();

        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);
    }

    void GLShader::compileGLProgram(const GLuint vertexShader, const GLuint fragmentShader) {
        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);
    }

    void GLShader::setUniformBool(const std::string& location, const bool value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), static_cast<GLint>(value));
    }

    void GLShader::setUniformFloat(const std::string& location, const float value) {
        glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::seUniformtInt(const std::string& location, const int value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::setVec2(const std::string& location, const glm::vec2& value) {
        glUniform2fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setVec3(const std::string& location, const glm::vec3& value) {
        glUniform3fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setVec4(const std::string& location, const glm::vec4& value) {
        glUniform4fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setMat2(const std::string& location, const glm::mat2& value) {
        glUniformMatrix2fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }

    void GLShader::setMat3(const std::string& location, const glm::mat3& value) {
        glUniformMatrix3fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }

    void GLShader::setMat4(const std::string& location, const glm::mat4& value) {
        glUniformMatrix4fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }
} // Kita
