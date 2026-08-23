#include "../kitapch.h"
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
            default:
                KITA_ENGINE_ASSERT(false, "Unknown or unsupported RenderingAPI");
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
            m_textureUniformsSet = true;
        }
    }

    const std::vector<Shader::ShaderInfo>& Shader::getShadersInfo() const {
        return m_shaders;
    }

    void Shader::replaceDefines(std::string& shaderSource, const std::vector<ShaderDefine>& defines, const std::string& shaderPath) {
        for (const auto& [define, value] : defines) {
            KITA_ENGINE_DEBUG("Replacing for shader {}: define: {} value: {}", shaderPath, define, value);
            StringUtil::replaceAll(shaderSource, define, value);
        }
    }
} // Kita
