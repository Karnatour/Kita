#pragma once
#include <glad/glad.h>

#include "../../../../Assets/Shader.h"

namespace Kita {
    class GLShader final : public Shader {
    public:
        ~GLShader() override;
        void bind() override;
        std::expected<void, ShaderError> createShader(std::span<const ShaderInfo> shaders) override;

        void setUniformBool(std::string_view location, bool value) override;
        void setUniformFloat(std::string_view location, float value) override;
        void setUniformInt(std::string_view location, int value) override;
        void setUniformUnsignedInt(std::string_view location, uint32_t value) override;
        void setUniformVec2(std::string_view location, const glm::vec2& value) override;
        void setUniformVec3(std::string_view location, const glm::vec3& value) override;
        void setUniformVec4(std::string_view location, const glm::vec4& value) override;
        void setUniformMat2(std::string_view location, const glm::mat2& value) override;
        void setUniformMat3(std::string_view location, const glm::mat3& value) override;
        void setUniformMat4(std::string_view location, const glm::mat4& value) override;
        GLint getUniformLocation(std::string_view location);

    private:
        GLenum getGLShaderType(ShaderType type);
        void releaseCompiledShaders(const std::vector<unsigned int>& glShaders);
        std::expected<void, ShaderError> compileGLShader(const GLuint& shader, const std::filesystem::path& shaderPath, const std::vector<ShaderDefine>& defines);
        std::expected<void, ShaderError> linkGLProgram(const std::vector<GLuint>& glShaders);
    };
} // Kita
