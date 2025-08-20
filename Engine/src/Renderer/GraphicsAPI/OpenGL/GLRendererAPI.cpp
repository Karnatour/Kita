#include "../../../kitapch.h"
#include "GLRendererAPI.h"

#include <glad/glad.h>

namespace Kita {
    void GLRendererAPI::setupDebug() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
        KITA_ENGINE_DEBUG("OpenGL Debug callback funtion is set");
    }

    void GLRendererAPI::setViewport(const int width, const int height) {
        glViewport(0, 0, width, height);
    }

    void GLRendererAPI::render(Model& model) {
        for (const auto& mesh : model.getMeshes()) {
            const auto materialIndex = mesh->getMaterialIndex();
            if (materialIndex >= 0 && materialIndex < model.getMaterials().size()) {
                const auto& material = model.getMaterials()[materialIndex];

                if (!material->getTextures().empty()) {
                    material->getTextures().at(0)->bind(0);
                }

                material->getShader()->bind();
            }

            mesh->getVertexArray()->bind();
            const auto& vertexArray = mesh->getVertexArray();
            if (vertexArray->getIBOobj()->getIndicesCount() == 0) {
                glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexArray->getVBOobj()->getVerticiesCount()));
            }
            else {
                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->getIBOobj()->getIndicesCount()),GL_UNSIGNED_INT, nullptr);
            }
        }
    }

    void GLRendererAPI::clearColor(const float red, const float green, const float blue, const float alpha) {
        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GLRendererAPI::debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message, void const* user_param) {
        auto const srcMsg = [source]() {
            switch (source) {
                case GL_DEBUG_SOURCE_API:
                    return "API";
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    return "WINDOW SYSTEM";
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    return "SHADER COMPILER";
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    return "THIRD PARTY";
                case GL_DEBUG_SOURCE_APPLICATION:
                    return "APPLICATION";
                case GL_DEBUG_SOURCE_OTHER:
                    return "OTHER";
                default:
                    KITA_ENGINE_WARN("Undefined OpenGL debug source");
                    return "UNKOWN GL SOURCE";
            }
        }();

        auto const typeMsg = [type]() {
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    return "ERROR";
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    return "DEPRECATED_BEHAVIOR";
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    return "UNDEFINED_BEHAVIOR";
                case GL_DEBUG_TYPE_PORTABILITY:
                    return "PORTABILITY";
                case GL_DEBUG_TYPE_PERFORMANCE:
                    return "PERFORMANCE";
                case GL_DEBUG_TYPE_MARKER:
                    return "MARKER";
                case GL_DEBUG_TYPE_OTHER:
                    return "OTHER";
                default:
                    KITA_ENGINE_WARN("Undefined OpenGL debug type");
                    return "UNKOWN GL TYPE";
            }
        }();

        switch (severity) {
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                KITA_ENGINE_DEBUG("{} {} {} {}", srcMsg, typeMsg, id, message);
                break;
            case GL_DEBUG_SEVERITY_LOW:
                KITA_ENGINE_INFO("{} {} {} {}", srcMsg, typeMsg, id, message);
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                KITA_ENGINE_WARN("{} {} {} {}", srcMsg, typeMsg, id, message);
                break;
            case GL_DEBUG_SEVERITY_HIGH:
                KITA_ENGINE_ERROR("{} {} {} {}", srcMsg, typeMsg, id, message);
                break;
            default:
                KITA_ENGINE_WARN("Undefined OpenGL debug severity");
                break;
        }
    };
} // Kita
