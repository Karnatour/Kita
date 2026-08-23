#pragma once
#include <expected>
#include <string>
#include "../Core/DllTemplate.h"
#include <memory>
#include <glm/glm.hpp>
#include <span>
#include "Asset.h"
#include "../Util/StringHash.h"

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
            return {.path = std::move(path), .type = ShaderType::VERTEX, .defines = std::move(defines)};
        }
        static ShaderInfo frag(std::filesystem::path path, std::vector<ShaderDefine> defines = {}) {
            return {.path = std::move(path), .type = ShaderType::FRAGMENT, .defines = std::move(defines)};
        }
        static ShaderInfo geom(std::filesystem::path path, std::vector<ShaderDefine> defines = {}) {
            return {.path = std::move(path), .type = ShaderType::GEOMETRY, .defines = std::move(defines)};
        }
        static ShaderDefine define(std::string name, std::string value) {
            return {.define = std::move(name), .value = std::move(value)};
        }

        virtual ~Shader() = default;
        virtual void bind() = 0;
        unsigned int getProgram() const;
        static std::unique_ptr<Shader> createPtr();

        void setTextureUniforms();

        virtual std::expected<void, ShaderError> createShader(std::span<const ShaderInfo> shaders) = 0;

        virtual void setUniformBool(std::string_view location, bool value) = 0;
        virtual void setUniformFloat(std::string_view location, float value) = 0;
        virtual void setUniformInt(std::string_view location, int value) = 0;
        virtual void setUniformUnsignedInt(std::string_view location, uint32_t value) = 0;
        virtual void setUniformVec2(std::string_view location, const glm::vec2& value) = 0;
        virtual void setUniformVec3(std::string_view location, const glm::vec3& value) = 0;
        virtual void setUniformVec4(std::string_view location, const glm::vec4& value) = 0;
        virtual void setUniformMat2(std::string_view location, const glm::mat2& value) = 0;
        virtual void setUniformMat3(std::string_view location, const glm::mat3& value) = 0;
        virtual void setUniformMat4(std::string_view location, const glm::mat4& value) = 0;

        const std::vector<ShaderInfo>& getShadersInfo() const;

    protected:
        void replaceDefines(std::string& shaderSource, const std::vector<ShaderDefine>& defines, const std::string& shaderPath);

        unsigned int m_program = 0;
        static inline unsigned int m_currentlyBoundProgram = 0;
        bool m_textureUniformsSet = false;
        std::vector<ShaderInfo> m_shaders;
        std::unordered_map<std::string, int, StringHash, std::equal_to<>> m_shaderLocations;
    };
} // Kita
