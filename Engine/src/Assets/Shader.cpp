#include "Shader.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/GraphicsAPI/OpenGL/GLShaders/GLShader.h"

namespace Kita {
    unsigned int Shader::getProgram() const {
        return m_program;
    }

    std::unique_ptr<Shader> Shader::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLShader>();
            case RenderingAPI::NONE:
                KITA_ENGINE_ASSERT(false, "RenderingAPI::NONE is not a valid selection");
                return nullptr;
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
                return nullptr;
        }
    }

    void Shader::setTextureUniforms() {
        if (!m_textureUniformsSet) {
            setUniformInt("albedoTex", 0);
            setUniformInt("metallicRoughnessTex", 1);
            setUniformInt("cubemapTex", 2);
            setUniformInt("colorTex", 3);
            setUniformInt("depthTex", 4);
            setUniformInt("stencilTex", 5);
            setUniformInt("skyboxTex", 6);
            setUniformInt("normalTex", 7);
            setUniformInt("depthTexArray", 8);
            setUniformInt("displacementTex", 9);
            setUniformInt("ambientOcclusionTex", 10);
            m_textureUniformsSet = true;
        }
    }

    std::span<const Shader::ShaderInfo> Shader::getShadersInfo() const {
        return m_shaderInfos;
    }

    std::expected<void, Shader::ShaderError> Shader::replaceDefines(std::string& shaderSource, const std::vector<ShaderDefine>& defines, const std::string& shaderPath) {
        const size_t versionPos = shaderSource.find("#version");
        if (versionPos == std::string::npos) {
            return std::unexpected(ShaderError{.code = ShaderErrorCode::REPLACE, .file = shaderPath});
        }

        const size_t insertPos = shaderSource.find('\n', versionPos);
        if (insertPos == std::string::npos) {
            return std::unexpected(ShaderError{.code = ShaderErrorCode::REPLACE, .file = shaderPath});
        }

        std::string block = "\n";
        for (const auto& [define, value] : defines) {
            KITA_ENGINE_DEBUG("Replacing for shader {}: define: {} value: {}", shaderPath, define, value);
            block = block + "#define " + define + " " + value + "\n";
        }
        block += "#line 2"; // Corrects line number when shader compilation fails

        shaderSource.insert(insertPos, block);

        return {};
    }
} // Kita
