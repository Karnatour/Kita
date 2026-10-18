#include "GLRendererAPI.h"
#include "../../Scene/ECS/Entity.h"
#include <glad/glad.h>
#include <magic_enum/magic_enum.hpp>

namespace Kita {
    GLenum GLRendererAPI::convertBufferTypeToInternalFormat(const BufferType buffer) {
        switch (buffer) {
            case BufferType::COLOR:
                return GL_RGBA8;
            case BufferType::DEPTH:
                return GL_DEPTH_COMPONENT24;
            case BufferType::DEPTH_STENCIL:
                return GL_DEPTH24_STENCIL8;
            default:
                KITA_ENGINE_ERROR("Unknown bufferType when trying to convert BufferType to GL Internal format");
                return GL_NONE;
        }
    }

    GLenum GLRendererAPI::convertCapabilityToGL(const Capability capability) {
        switch (capability) {
            case Capability::DEPTH_TEST:
                return GL_DEPTH_TEST;
            case Capability::CULL_FACE:
                return GL_CULL_FACE;
            default:
                KITA_ENGINE_ERROR("Trying to convert unknown capability to GL variation {} ", magic_enum::enum_name(capability));
                return GL_NONE;
        }
    }

    GLenum GLRendererAPI::convertDrawModeToGL(const DrawMode drawMode) {
        switch (drawMode) {
            case DrawMode::POINTS:
                return GL_POINTS;
            case DrawMode::LINES:
                return GL_LINES;
            case DrawMode::TRIANGLES:
                return GL_TRIANGLES;
            default:
                KITA_ENGINE_ERROR("Trying to convert unknown capability to GL variation {} ", magic_enum::enum_name(drawMode));
                return GL_NONE;
        }
    }

    GLbitfield GLRendererAPI::convertBitToGL(const ClearBit bit) {
        switch (bit) {
            case ClearBit::COLOR:
                return GL_COLOR_BUFFER_BIT;
            case ClearBit::DEPTH:
                return GL_DEPTH_BUFFER_BIT;
            case ClearBit::STENCIL:
                return GL_STENCIL_BUFFER_BIT;
            default:
                KITA_ENGINE_ERROR("Trying to convert unknown capability to GL variation {} ", magic_enum::enum_name(bit));
                return GL_NONE;
        }
    }

    void GLRendererAPI::setupDebug() {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(debugCallback, nullptr);
        KITA_ENGINE_DEBUG("OpenGL Debug callback function has been set");
    }

    void GLRendererAPI::setViewport(const std::pair<int, int> resolution) {
        glViewport(0, 0, resolution.first, resolution.second);
    }

    void GLRendererAPI::clearColor(const float red, const float green, const float blue, const float alpha) {
        glClearColor(red, green, blue, alpha);
    }

    void GLRendererAPI::clearBit(const std::span<const ClearBit> bits) {
        GLbitfield mask = 0;
        for (const auto bit : bits) {
            mask |= convertBitToGL(bit);
        }
        glClear(mask);
    }

    void GLRendererAPI::enableCapability(const Capability capability) {
        glEnable(convertCapabilityToGL(capability));
    }

    void GLRendererAPI::disableCapability(const Capability capability) {
        glDisable(convertCapabilityToGL(capability));
    }

    void GLRendererAPI::enableBufferWrite(const BufferType bufferType) {
        switch (bufferType) {
            case BufferType::COLOR:
                //Enable all channels
                glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
                break;
            case BufferType::DEPTH:
                glDepthMask(GL_TRUE);
                break;
            case BufferType::STENCIL:
                glStencilMask(GL_TRUE);
                break;
            case BufferType::DEPTH_STENCIL:
                glDepthMask(GL_TRUE);
                glStencilMask(GL_TRUE);
                break;
        }
    }

    void GLRendererAPI::disableBufferWrite(const BufferType bufferType) {
        switch (bufferType) {
            case BufferType::COLOR:
                //Disable all channels
                glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
                break;
            case BufferType::DEPTH:
                glDepthMask(GL_FALSE);
                break;
            case BufferType::STENCIL:
                glStencilMask(GL_FALSE);
                break;
            case BufferType::DEPTH_STENCIL:
                glDepthMask(GL_FALSE);
                glStencilMask(GL_FALSE);
                break;
        }
    }

    void GLRendererAPI::setDepthFunc(const DepthFunction function) {
        switch (function) {
            case DepthFunction::NEVER:
                glDepthFunc(GL_NEVER);
                break;
            case DepthFunction::LESS:
                glDepthFunc(GL_LESS);
                break;
            case DepthFunction::EQUAL:
                glDepthFunc(GL_EQUAL);
                break;
            case DepthFunction::LEQUAL:
                glDepthFunc(GL_LEQUAL);
                break;
            case DepthFunction::GREATER:
                glDepthFunc(GL_GREATER);
                break;
            case DepthFunction::NOTEQUAL:
                glDepthFunc(GL_NOTEQUAL);
                break;
            case DepthFunction::GEQUAL:
                glDepthFunc(GL_GEQUAL);
                break;
            case DepthFunction::ALWAYS:
                glDepthFunc(GL_ALWAYS);
                break;
        }
    }

    void GLRendererAPI::setCullMode(const CullMode mode) {
        switch (mode) {
            case CullMode::FRONT:
                glCullFace(GL_FRONT);
                break;
            case CullMode::BACK:
                glCullFace(GL_BACK);
                break;
            case CullMode::FRONT_BACK:
                glCullFace(GL_FRONT_AND_BACK);
                break;
        }
    }

    void GLRendererAPI::drawArrays(const DrawMode mode, const size_t verticesCount) {
        KITA_ENGINE_PROFILE("Draw Arrays");

        glDrawArrays(convertDrawModeToGL(mode), 0, static_cast<GLsizei>(verticesCount));
    }

    void GLRendererAPI::multiDrawArraysIndirect(const DrawMode mode, const size_t drawCount) {
        KITA_ENGINE_PROFILE("Multi Draw Arrays Indirect");

        glMultiDrawArraysIndirect(convertDrawModeToGL(mode), nullptr, static_cast<GLsizei>(drawCount), 0);
    }

    void GLRendererAPI::drawElements(const DrawMode mode, const size_t indicesCount) {
        KITA_ENGINE_PROFILE("Draw Elements");

        glDrawElements(convertDrawModeToGL(mode), static_cast<GLsizei>(indicesCount),GL_UNSIGNED_INT, nullptr);
    }

    void GLRendererAPI::multiDrawElementsIndirect(const DrawMode mode, const size_t drawCount) {
        KITA_ENGINE_PROFILE("Multi Draw Elements Indirect");

        glMultiDrawElementsIndirect(convertDrawModeToGL(mode),GL_UNSIGNED_INT, nullptr, static_cast<GLsizei>(drawCount), 0);
    }

    void GLRendererAPI::debugCallback(GLenum source, GLenum type, GLuint id, const GLenum severity, GLsizei /*length*/, GLchar const* message, void const* /*user_param*/) {
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
                KITA_ENGINE_ASSERT(false, "OpenGL High severiy error");
                break;
            default:
                KITA_ENGINE_WARN("Undefined OpenGL debug severity");
                break;
        }
    }
} // Kita
