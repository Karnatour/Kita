#include "../../../kitapch.h"
#include "GLVertexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    void GLVertexBuffer::create(unsigned size, std::vector<Vertex>& vertices) {
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
