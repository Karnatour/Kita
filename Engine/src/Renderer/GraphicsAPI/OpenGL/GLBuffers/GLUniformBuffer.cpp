#include "../../../../kitapch.h"
#include "GLUniformBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLUniformBuffer::~GLUniformBuffer() {
        glDeleteBuffers(1, &m_ubo);
    }

    void GLUniformBuffer::bind(const unsigned int position) {
        glBindBufferBase(GL_UNIFORM_BUFFER, position, m_ubo);
    }

    void GLUniformBuffer::upload(const unsigned int size, const void* data) {
        if (size == m_size) {
            glNamedBufferSubData(m_ubo, 0, size, data);
        }
        else {
            KITA_ENGINE_WARN("Input size doesn't match with uniform buffer size Before:{} New:{}", m_size, size);
        }
    }

    void GLUniformBuffer::createBuffer(const unsigned int size, const void* data) {
        m_size = size;
        glCreateBuffers(1, &m_ubo);
        glNamedBufferStorage(m_ubo, m_size, data,GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
