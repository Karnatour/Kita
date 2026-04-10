#include "../../../../kitapch.h"
#include "GLVertexArray.h"
#include "../../../Vertex.h"
#include <glad/glad.h>

namespace Kita {
    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &m_vao);
    }

    void GLVertexArray::createBuffer(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo) {
        glCreateVertexArrays(1, &m_vao);

        glVertexArrayVertexBuffer(m_vao, 0, vbo->getVBO(), 0, sizeof(Vertex));
        if (ibo != nullptr) {
            glVertexArrayElementBuffer(m_vao, ibo->getIBO());
        }

        //Position
        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, position));
        glVertexArrayAttribBinding(m_vao, 0, 0);

        //Color
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 1, 4,GL_FLOAT,GL_FALSE,offsetof(Vertex, color));
        glVertexArrayAttribBinding(m_vao, 1, 0);

        //Texture
        glEnableVertexArrayAttrib(m_vao, 2);
        glVertexArrayAttribFormat(m_vao, 2, 2,GL_FLOAT,GL_FALSE,offsetof(Vertex, texture));
        glVertexArrayAttribBinding(m_vao, 2, 0);

        //Normal
        glEnableVertexArrayAttrib(m_vao, 3);
        glVertexArrayAttribFormat(m_vao, 3, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, normal));
        glVertexArrayAttribBinding(m_vao, 3, 0);
    }

    void GLVertexArray::bind() {
        glBindVertexArray(m_vao);
    }
} // Kita
