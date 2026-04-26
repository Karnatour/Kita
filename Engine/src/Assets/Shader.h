#pragma once
#include <expected>
#include <string>
#include "../Core/DllTemplate.h"
#include <memory>
#include <glm/glm.hpp>
#include <span>
#include "Asset.h"

namespace Kita {
    class KITAENGINE_API Shader : public Asset {
    public:
        enum class ShaderErrorCode {
            FILE,
            COMPILATION,
            LINKING
        };

        enum class ShaderType {
            VERTEX,
            FRAGMENT,
            GEOMETRY,
        };

        struct ShaderError {
            ShaderErrorCode code;
            std::string file;
        };

        struct ShaderDefine {
            std::string define;
            std::string value;
        };

        struct ShaderInfo {
            std::filesystem::path path;
            ShaderType type;
            std::vector<ShaderDefine> defines;
        };

        static ShaderInfo vert(std::filesystem::path path, std::vector<ShaderDefine> defines = {}) {
            return {std::move(path), ShaderType::VERTEX, std::move(defines)};
        }
        static ShaderInfo frag(std::filesystem::path path, std::vector<ShaderDefine> defines = {}) {
            return {std::move(path), ShaderType::FRAGMENT, std::move(defines)};
        }
        static ShaderInfo geom(std::filesystem::path path, std::vector<ShaderDefine> defines = {}) {
            return {std::move(path), ShaderType::GEOMETRY, std::move(defines)};
        }
        static ShaderDefine define(std::string name, std::string value) {
            return {std::move(name), std::move(value)};
        }

        virtual ~Shader() = default;
        virtual void bind() = 0;
        unsigned int getProgram() const;
        static std::unique_ptr<Shader> createPtr();
        virtual std::expected<void, ShaderError> createShader(std::span<const ShaderInfo> shaders) = 0;

        virtual void setUniformBool(const std::string& location, bool value) = 0;
        virtual void setUniformFloat(const std::string& location, float value) = 0;
        virtual void setUniformInt(const std::string& location, int value) = 0;
        virtual void setUniformVec2(const std::string& location, const glm::vec2& value) = 0;
        virtual void setUniformVec3(const std::string& location, const glm::vec3& value) = 0;
        virtual void setUniformVec4(const std::string& location, const glm::vec4& value) = 0;
        virtual void setUniformMat2(const std::string& location, const glm::mat2& value) = 0;
        virtual void setUniformMat3(const std::string& location, const glm::mat3& value) = 0;
        virtual void setUniformMat4(const std::string& location, const glm::mat4& value) = 0;

        const std::vector<ShaderInfo>& getShadersInfo() const;

    protected:
        void replaceDefines(std::string& shaderSource, const std::vector<ShaderDefine>& defines, const std::string& shaderPath);

        unsigned int m_program = 0;
        std::vector<ShaderInfo> m_shaders;
    };
} // Kita
