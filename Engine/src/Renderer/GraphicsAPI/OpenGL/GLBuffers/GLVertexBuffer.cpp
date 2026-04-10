#include "../../../../kitapch.h"
#include "GLVertexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &m_vbo);
    }

    void GLVertexBuffer::createBuffer(std::vector<Vertex> vertices) {
        m_vertices = std::move(vertices);
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, static_cast<GLsizeiptr>(sizeof(Vertex) * m_vertices.size()), m_vertices.data(),GL_DYNAMIC_STORAGE_BIT);
    }
} // Kita
