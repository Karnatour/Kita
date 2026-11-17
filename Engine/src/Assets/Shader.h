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
            LINKING,
            REPLACE,
            SIZE_MISMATCH
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

        struct ShaderInfoInput {
            ShaderType type;
            std::vector<ShaderDefine> defines;
        };

        static ShaderInfoInput vert(std::vector<ShaderDefine> defines = {}) {
            return {.type = ShaderType::VERTEX, .defines = std::move(defines)};
        }
        static ShaderInfoInput frag(std::vector<ShaderDefine> defines = {}) {
            return {.type = ShaderType::FRAGMENT, .defines = std::move(defines)};
        }
        static ShaderInfoInput geom(std::vector<ShaderDefine> defines = {}) {
            return {.type = ShaderType::GEOMETRY, .defines = std::move(defines)};
        }

        static ShaderDefine define(std::string name, std::string value) {
            return {.define = std::move(name), .value = std::move(value)};
        }

        ~Shader() override = default;
        virtual void bind() = 0;
        unsigned int getProgram() const;
        static std::unique_ptr<Shader> createPtr();

        void setTextureUniforms();

        virtual std::expected<void, ShaderError> createShader(std::vector<std::filesystem::path> shaderPaths, std::vector<ShaderInfoInput> shaderInfos) = 0;

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

        std::span<const ShaderInfo> getShadersInfo() const;

    protected:
        std::expected<void, ShaderError> replaceDefines(std::string& shaderSource, const std::vector<ShaderDefine>& defines, const std::string& shaderPath);

        unsigned int m_program = 0;
        static inline unsigned int m_currentlyBoundProgram = 0;
        bool m_textureUniformsSet = false;
        std::vector<ShaderInfo> m_shaderInfos;
        std::unordered_map<std::string, int, StringHash, std::equal_to<>> m_shaderLocations;
    };
} // Kita
