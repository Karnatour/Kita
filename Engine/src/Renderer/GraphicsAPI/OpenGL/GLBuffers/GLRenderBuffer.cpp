#include "GLRenderBuffer.h"

namespace Kita {
    GLRenderBuffer::~GLRenderBuffer() {
        glDeleteRenderbuffers(1, &m_rbo);
    }

    void GLRenderBuffer::createBuffer(const std::pair<int, int>& resolution, const BufferType bufferType) {
        m_resolution = resolution;
        glCreateRenderbuffers(1, &m_rbo);
        glNamedRenderbufferStorage(m_rbo, convertBufferTypeToInternalFormat(bufferType), m_resolution.first, m_resolution.second);
    }

    void GLRenderBuffer::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    }

    GLenum GLRenderBuffer::convertBufferTypeToInternalFormat(const BufferType buffer) {
        switch (buffer) {
            case BufferType::COLOR:
                return GL_RGBA8;
            case BufferType::DEPTH:
                return GL_DEPTH_COMPONENT24;
            case BufferType::STENCIL:
                return GL_STENCIL_INDEX8;
            default:
                KITA_ENGINE_ERROR("Unknown bufferType when trying to convert BufferType to GL Internal format");
                return GL_NONE;
        }
    }
} // Kita
