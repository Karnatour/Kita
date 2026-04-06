#include "../kitapch.h"
#include "../Renderer/Shaders/Shader.h"

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

    const std::pair<std::filesystem::path, std::filesystem::path>& Shader::getPath() const {
        return m_path;
    }
} // Kita
