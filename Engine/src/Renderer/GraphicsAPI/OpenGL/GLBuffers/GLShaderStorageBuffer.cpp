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

    void GLShaderStorageBuffer::update(unsigned size, const void* data) {
        GLint bufferSize = 0;
        glGetNamedBufferParameteriv(m_ssbo, GL_BUFFER_SIZE, &bufferSize);
        if (size > static_cast<unsigned>(bufferSize)) {
            // Reallocate
            KITA_ENGINE_DEBUG("SSBO is too small, reallocating, old:{} new:{}", bufferSize, size);
            glNamedBufferData(m_ssbo, size, data, GL_DYNAMIC_DRAW);
        }
        else {
            glNamedBufferSubData(m_ssbo, 0, size, data);
        }
    }

    void GLShaderStorageBuffer::createBuffer(const unsigned size, const void* data) {
        glCreateBuffers(1, &m_ssbo);
        glNamedBufferData(m_ssbo, size, data,GL_DYNAMIC_DRAW);
    }
} // Kita
