#include "../../../../kitapch.h"
#include "GLShader.h"

#include "../../../../Assets/AssetManager.h"

namespace Kita {
    void GLShader::bind() {
        glUseProgram(m_program);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_program);
    }

    std::expected<void, Shader::ShaderError> GLShader::createShader(const std::span<const ShaderInfo> shaders) {
        m_shaders = std::ranges::to<std::vector>(shaders);
        KITA_ENGINE_DEBUG("Starting compilation of {} shaders", shaders.size());

        std::vector<GLuint> glShaders;
        for (const auto& [path, type, defines] : m_shaders) {
            GLuint shaderID = glCreateShader(getGLShaderType(type));
            if (auto result = compileGLShader(shaderID, AssetManager::SHADER_PREFIX / path, defines); !result) {
                // Release already compiled shaders if compilation fails
                releaseCompiledShaders(glShaders);
                return result;
            }
            glShaders.push_back(shaderID);
        }

        auto result = linkGLProgram(glShaders);
        releaseCompiledShaders(glShaders);

        return result;
    }

    void GLShader::releaseCompiledShaders(const std::vector<unsigned int>& glShaders) {
        for (const auto& shader : glShaders) {
            glDeleteShader(shader);
        }
    }

    std::expected<void, Shader::ShaderError> GLShader::compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath, const std::vector<ShaderDefine>& defines) {
        const std::optional<std::string> shaderSource = FileReader::readFile(shaderPath);
        std::string shaderPathStr = shaderPath.string();

        if (!shaderSource.has_value()) {
            return std::unexpected(ShaderError(ShaderErrorCode::FILE, shaderPathStr));
        }

        std::string shaderSourceStr = shaderSource.value();
        replaceDefines(shaderSourceStr, defines, shaderPathStr);

        const char* sourcePtr = shaderSourceStr.data();

        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            KITA_ENGINE_ERROR("GLShader compilation failed ({}): {}", shaderPathStr, log);
            return std::unexpected(ShaderError(ShaderErrorCode::COMPILATION, shaderPathStr));
        }
        return {};
    }

    std::expected<void, Shader::ShaderError> GLShader::linkGLProgram(const std::vector<GLuint>& glShaders) {
        m_program = glCreateProgram();
        for (const auto shader : glShaders) {
            glAttachShader(m_program, shader);
        }
        glLinkProgram(m_program);

        GLint success;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, ' ');
            glGetProgramInfoLog(m_program, logLength, nullptr, log.data());
            KITA_ENGINE_ERROR("GLProgram linking failed: {}", log);
            return std::unexpected(ShaderError(ShaderErrorCode::LINKING));
        }

        return {};
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

    GLenum GLShader::getGLShaderType(const ShaderType type) {
        switch (type) {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;
            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;
            case ShaderType::GEOMETRY:
                return GL_GEOMETRY_SHADER;
        }
        KITA_ENGINE_ASSERT(false, "Trying to convert invalid ShaderType");
    }
} // Kita
