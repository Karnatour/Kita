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

    void GLUniformBuffer::update(const unsigned int size, const void* data) {
        GLint bufferSize = 0;
        glGetNamedBufferParameteriv(m_ubo, GL_BUFFER_SIZE, &bufferSize);
        if (size == bufferSize) {
            glNamedBufferSubData(m_ubo, 0, size, data);
        }
        else {
            KITA_ENGINE_WARN("Input size doesn't match with uniform buffer size Before:{} New:{}",bufferSize,size);
        }
    }

    void GLUniformBuffer::createBuffer(const unsigned int size, const void* data) {
        glCreateBuffers(1, &m_ubo);
        glNamedBufferStorage(m_ubo, size, data,GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
