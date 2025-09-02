#include "../../../../kitapch.h"
#include "GLShader.h"

#include "../../../Shaders/ShaderManager.h"

namespace Kita {
    void GLShader::bind() {
        glUseProgram(m_program);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_program);
    }

    void GLShader::compileShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        m_path = std::make_pair(vertexPath, fragmentPath);
        KITA_ENGINE_DEBUG("Starting compilation of shaders, [VERTEX]->{}, [FRAGMENT]->{}", m_path.first.string(), m_path.second.string());

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        compileGLShader(vertexShader, ShaderManager::SHADER_PREFIX / m_path.first);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        compileGLShader(fragmentShader, ShaderManager::SHADER_PREFIX / m_path.second);

        compileGLProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void GLShader::compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath) {
        const std::string& shaderSource = FileReader::readFile(shaderPath);
        const char* sourcePtr = shaderSource.data();

        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            KITA_ENGINE_ERROR("Shader compilation failed ({}): {}", shaderPath.string(), log);
        }
    }

    void GLShader::compileGLProgram(const GLuint vertexShader, const GLuint fragmentShader) {
        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);

        GLint success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
            KITA_ENGINE_ERROR("Shader program linking failed: {}", log);
        }
    }

    void GLShader::setUniformBool(const std::string& location, const bool value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), static_cast<GLint>(value));
    }

    void GLShader::setUniformFloat(const std::string& location, const float value) {
        glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::setUniformInt(const std::string& location, const int value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::setUniformVec2(const std::string& location, const glm::vec2& value) {
        glUniform2fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setUniformVec3(const std::string& location, const glm::vec3& value) {
        glUniform3fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setUniformVec4(const std::string& location, const glm::vec4& value) {
        glUniform4fv(glGetUniformLocation(m_program, location.c_str()), 1, &value[0]);
    }

    void GLShader::setUniformMat2(const std::string& location, const glm::mat2& value) {
        glUniformMatrix2fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }

    void GLShader::setUniformMat3(const std::string& location, const glm::mat3& value) {
        glUniformMatrix3fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }

    void GLShader::setUniformMat4(const std::string& location, const glm::mat4& value) {
        glUniformMatrix4fv(glGetUniformLocation(m_program, location.c_str()), 1,GL_FALSE, &value[0][0]);
    }
} // Kita
