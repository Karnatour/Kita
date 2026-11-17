#include "GLIndexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLIndexBuffer::GLIndexBuffer(std::vector<unsigned int> indices) : IndexBuffer(std::move(indices)) {
        KITA_ENGINE_ASSERT(!m_indices.empty(), "IndexBuffer has received empty indices vector");

        glCreateBuffers(1, &m_ibo);
        glNamedBufferStorage(m_ibo, static_cast<GLsizeiptr>(sizeof(unsigned int) * m_indices.size()), m_indices.data(),GL_DYNAMIC_STORAGE_BIT);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        if (m_ibo != 0) {
            glDeleteBuffers(1, &m_ibo);
            m_ibo = 0;
        }
    }
} // Kita
