#include "GLRenderBuffer.h"

#include "../GLRendererAPI.h"

namespace Kita {
    GLRenderBuffer::GLRenderBuffer(const std::pair<int, int>& resolution, const BufferType bufferType) : RenderBuffer(resolution, bufferType) {
        glCreateRenderbuffers(1, &m_rbo);
        glNamedRenderbufferStorage(m_rbo, GLRendererAPI::convertBufferTypeToInternalFormat(bufferType), m_resolution.first, m_resolution.second);
    }

    GLRenderBuffer::~GLRenderBuffer() {
        if (m_rbo != 0) {
            glDeleteRenderbuffers(1, &m_rbo);
            m_rbo = 0;
        }
    }

    void GLRenderBuffer::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    }

    void GLRenderBuffer::destroy() {
        glDeleteRenderbuffers(1, &m_rbo);
        m_rbo = 0;
    }
} // Kita
