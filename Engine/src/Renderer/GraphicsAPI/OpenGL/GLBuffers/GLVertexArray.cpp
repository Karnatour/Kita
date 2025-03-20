#include "GLVertexArray.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexArray::~GLVertexArray() {
        glDeleteBuffers(1,&m_vao);
    }

    void GLVertexArray::createBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo) {
        glCreateVertexArrays(1, &m_vao);

        glVertexArrayVertexBuffer(m_vao, 0, vbo.getVBO(), 0, sizeof(Vertex));
        glVertexArrayElementBuffer(m_vao, ibo.getEBO());

        //Position
        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, position));
        glVertexArrayAttribBinding(m_vao, 0, 0);

        //Color
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 1, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, color));
        glVertexArrayAttribBinding(m_vao, 1, 0);
    }

    void GLVertexArray::bind() {
        glBindVertexArray(m_vao);
    }
} // Kita
