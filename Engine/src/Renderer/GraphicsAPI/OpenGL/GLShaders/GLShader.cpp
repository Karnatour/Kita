#include "../../../../kitapch.h"
#include "GLShader.h"

#include "../../../../Assets/AssetManager.h"

namespace Kita {
    void GLShader::bind() {
        KITA_ENGINE_PROFILE("Bind shader");
        if (m_currentlyBoundProgram != m_program) {
            m_currentlyBoundProgram = m_program;
            glUseProgram(m_program);
        }
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_program);
        if (m_currentlyBoundProgram == m_program) {
            m_currentlyBoundProgram = 0;
        }
    }

    std::expected<void, Shader::ShaderError> GLShader::createShader(const std::span<const ShaderInfo> shaders) {
        m_shaders = std::ranges::to<std::vector>(shaders);
        KITA_ENGINE_DEBUG("Starting compilation of {} shaders", shaders.size());
        for (const auto& [index, shader] : std::views::enumerate(m_shaders)) {
            KITA_ENGINE_DEBUG("{}: {}", index, shader.path.string());
        }

        std::vector<GLuint> glShaders;
        for (const auto& [path, type, defines] : m_shaders) {
            GLuint shaderID = glCreateShader(getGLShaderType(type));
            if (auto result = compileGLShader(shaderID, AssetManager::SHADER_PREFIX / path, defines); !result) {
                // Release already compiled shaders if compilation fails
                releaseCompiledShaders(glShaders);
                return result;
            }
            glShaders.emplace_back(shaderID);
        }

        auto result = linkGLProgram(glShaders);
        releaseCompiledShaders(glShaders);

        KITA_ENGINE_DEBUG("Finished compilation of {} shaders", shaders.size());
        return result;
    }

    void GLShader::releaseCompiledShaders(const std::vector<unsigned int>& glShaders) {
        for (const auto& shader : glShaders) {
            glDeleteShader(shader);
        }
    }

    std::expected<void, Shader::ShaderError> GLShader::compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath, const std::vector<ShaderDefine>& defines) {
        const std::optional<std::string> shaderSource = FileReader::readFileString(shaderPath);
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

    void GLShader::setUniformBool(const std::string_view location, const bool value) {
        KITA_ENGINE_PROFILE("Set uniform bool");
        glUniform1f(getUniformLocation(location), value);
    }

    void GLShader::setUniformFloat(const std::string_view location, const float value) {
        KITA_ENGINE_PROFILE("Set uniform float");
        glUniform1f(getUniformLocation(location), value);
    }

    void GLShader::setUniformInt(const std::string_view location, const int value) {
        KITA_ENGINE_PROFILE("Set uniform int");
        glUniform1i(getUniformLocation(location), value);
    }

    void GLShader::setUniformUnsignedInt(const std::string_view location, const uint32_t value) {
        KITA_ENGINE_PROFILE("Set uniform unsigned int");
        glUniform1ui(getUniformLocation(location), value);
    }

    void GLShader::setUniformVec2(const std::string_view location, const glm::vec2& value) {
        KITA_ENGINE_PROFILE("Set uniform vec2");
        glUniform2fv(getUniformLocation(location), 1, &value[0]);
    }

    void GLShader::setUniformVec3(const std::string_view location, const glm::vec3& value) {
        KITA_ENGINE_PROFILE("Set uniform vec3");
        glUniform3fv(getUniformLocation(location), 1, &value[0]);
    }

    void GLShader::setUniformVec4(const std::string_view location, const glm::vec4& value) {
        KITA_ENGINE_PROFILE("Set uniform vec4");
        glUniform4fv(getUniformLocation(location), 1, &value[0]);
    }

    void GLShader::setUniformMat2(const std::string_view location, const glm::mat2& value) {
        KITA_ENGINE_PROFILE("Set uniform mat2");
        glUniformMatrix2fv(getUniformLocation(location), 1, GL_FALSE, &value[0][0]);
    }

    void GLShader::setUniformMat3(const std::string_view location, const glm::mat3& value) {
        KITA_ENGINE_PROFILE("Set uniform mat3");
        glUniformMatrix3fv(getUniformLocation(location), 1,GL_FALSE, &value[0][0]);
    }

    void GLShader::setUniformMat4(const std::string_view location, const glm::mat4& value) {
        KITA_ENGINE_PROFILE("Set uniform mat4");
        glUniformMatrix4fv(getUniformLocation(location), 1,GL_FALSE, &value[0][0]);
    }

    GLint GLShader::getUniformLocation(const std::string_view location) {
        KITA_ENGINE_PROFILE("Get uniform location");
        GLint glLocation;

        if (const auto it = m_shaderLocations.find(location); it != m_shaderLocations.end()) {
            glLocation = it->second;
        } else {
            glLocation = glGetUniformLocation(m_program, std::string(location).c_str());
            m_shaderLocations.try_emplace(std::string(location), glLocation);
        }
        return glLocation;
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
