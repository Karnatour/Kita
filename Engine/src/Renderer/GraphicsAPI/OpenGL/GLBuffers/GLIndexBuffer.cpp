#include "GLIndexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1,&m_ibo);
    }


    void GLIndexBuffer::createBuffer(const std::vector<unsigned int>& indicies) {
        m_indicesCount = indicies.size();
        glCreateBuffers(1, &m_ibo);
        glNamedBufferStorage(m_ibo, static_cast<GLsizeiptr>(sizeof(unsigned int) * indicies.size()), indicies.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
