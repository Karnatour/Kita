#include "../../kitapch.h"
#include "Shader.h"

#include "../Renderer.h"
#include "../GraphicsAPI/OpenGL/GLShaders/GLShader.h"

namespace Kita {
    unsigned int Shader::getProgramID() const {
        return m_program;
    }

    std::shared_ptr<Shader> Shader::createPtr() {
        switch (Renderer::getAPI()) {
        case RenderingAPI::OPENGL:
            return std::make_shared<GLShader>();
        default:
            KITA_ENGINE_ERROR("Trying to create Shader while RenderingAPI is not selected, returning nullptr");
            return nullptr;
        }
    }
} // Kita
