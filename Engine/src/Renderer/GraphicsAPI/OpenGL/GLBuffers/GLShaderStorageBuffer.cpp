#include "../../../../kitapch.h"
#include "GLShaderStorageBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLShaderStorageBuffer::~GLShaderStorageBuffer() {
        glDeleteBuffers(1, &m_ssbo);
    }

    void GLShaderStorageBuffer::bind(const unsigned position) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, position, m_ssbo);
    }

    void GLShaderStorageBuffer::upload(const unsigned size, const void* data) {
        if (size > m_size) {
            // Reallocate
            m_size = std::max(m_size * 2, size);
            KITA_ENGINE_DEBUG("SSBO is too small, reallocating, newSize:{}", m_size);
            glNamedBufferData(m_ssbo, m_size, data, GL_DYNAMIC_DRAW);
        }
        else {
            glNamedBufferSubData(m_ssbo, 0, size, data);
        }
    }

    void GLShaderStorageBuffer::createBuffer(const unsigned size, const void* data) {
        m_size = size;
        glCreateBuffers(1, &m_ssbo);
        glNamedBufferData(m_ssbo, m_size, data,GL_DYNAMIC_DRAW);
    }
} // Kita
