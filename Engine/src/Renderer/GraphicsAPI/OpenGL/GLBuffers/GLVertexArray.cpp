#include "../../../../kitapch.h"
#include "GLVertexArray.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexArray::~GLVertexArray() {
        glDeleteBuffers(1, &m_vao);
    }

    void GLVertexArray::createBuffer(const std::vector<Vertex>& verticies, const std::vector<unsigned int>& indices) {
        m_vbo->createBuffer(verticies);
        if (!indices.empty()) {
            m_ibo->createBuffer(indices);
        }

        glCreateVertexArrays(1, &m_vao);

        glVertexArrayVertexBuffer(m_vao, 0, m_vbo->getVBO(), 0, sizeof(Vertex));
        if (!indices.empty()) {
            glVertexArrayElementBuffer(m_vao, m_ibo->getIBO());
        }

        //Position
        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, position));
        glVertexArrayAttribBinding(m_vao, 0, 0);

        //Color
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 1, 3,GL_FLOAT,GL_FALSE,offsetof(Vertex, color));
        glVertexArrayAttribBinding(m_vao, 1, 0);

        //Texture
        glEnableVertexArrayAttrib(m_vao, 2);
        glVertexArrayAttribFormat(m_vao, 2, 2,GL_FLOAT,GL_FALSE,offsetof(Vertex, texture));
        glVertexArrayAttribBinding(m_vao, 2, 0);
    }

    void GLVertexArray::bind() {
        glBindVertexArray(m_vao);
    }
} // Kita
