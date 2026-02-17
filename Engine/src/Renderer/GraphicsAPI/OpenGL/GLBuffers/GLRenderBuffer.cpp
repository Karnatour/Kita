#include "GLRenderBuffer.h"

#include "../GLRendererAPI.h"

namespace Kita {
    GLRenderBuffer::~GLRenderBuffer() {
        glDeleteRenderbuffers(1, &m_rbo);
    }

    void GLRenderBuffer::createBuffer(const std::pair<int, int>& resolution, const BufferType bufferType) {
        m_resolution = resolution;
        glCreateRenderbuffers(1, &m_rbo);
        glNamedRenderbufferStorage(m_rbo, GLRendererAPI::convertBufferTypeToInternalFormat(bufferType), m_resolution.first, m_resolution.second);
    }

    void GLRenderBuffer::bind() {
        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    }

    void GLRenderBuffer::destroy() {
        glDeleteRenderbuffers(1, &m_rbo);
    }

} // Kita
