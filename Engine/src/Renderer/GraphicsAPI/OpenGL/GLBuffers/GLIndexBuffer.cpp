#include "../../../../kitapch.h"
#include "GLIndexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &m_ibo);
    }

    void GLIndexBuffer::createBuffer(std::vector<unsigned int> indices) {
        m_indices = std::move(indices);
        glCreateBuffers(1, &m_ibo);
        glNamedBufferStorage(m_ibo, static_cast<GLsizeiptr>(sizeof(unsigned int) * m_indices.size()), m_indices.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
