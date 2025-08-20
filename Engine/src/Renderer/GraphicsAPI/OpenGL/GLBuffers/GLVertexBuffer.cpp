#include "../../../../kitapch.h"
#include "GLVertexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1,&m_vbo);
    }

    void GLVertexBuffer::createBuffer(const std::vector<Vertex>& vertices) {
        m_vertices = vertices;
        m_verticiesCount = vertices.size();
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, static_cast<GLsizeiptr>(sizeof(Vertex) * vertices.size()), vertices.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
