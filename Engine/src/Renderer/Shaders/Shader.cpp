#include "../../kitapch.h"
#include "Shader.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLShaders/GLShader.h"

namespace Kita {
    unsigned int Shader::getProgram() const {
        return m_program;
    }

    std::shared_ptr<Shader> Shader::createPtr() {
        switch (Renderer::getAPI()) {
            case RenderingAPI::OPENGL:
                return std::make_shared<GLShader>();
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
