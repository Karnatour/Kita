#include "GLIndexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1,&m_ebo);
    }


    void GLIndexBuffer::createBuffer(const std::vector<unsigned int>& indicies) {
        glCreateBuffers(1, &m_ebo);
        glNamedBufferStorage(m_ebo, static_cast<GLsizeiptr>(sizeof(unsigned int) * indicies.size()), indicies.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
