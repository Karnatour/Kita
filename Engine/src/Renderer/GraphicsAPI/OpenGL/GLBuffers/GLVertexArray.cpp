#include "GLVertexArray.h"
#include "../../../Properties/VertexProperties.h"
#include <glad/glad.h>

namespace Kita {
    GLVertexArray::~GLVertexArray() {
        if (m_vao != 0) {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    void GLVertexArray::createBuffer(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo) {
        glCreateVertexArrays(1, &m_vao);

        if (vbo != nullptr && vbo->getVBO() != 0) {
            glVertexArrayVertexBuffer(m_vao, 0, vbo->getVBO(), 0, sizeof(VertexProperties));
        }
        if (ibo != nullptr && ibo->getIBO() != 0) {
            glVertexArrayElementBuffer(m_vao, ibo->getIBO());
        }

        //Position
        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, position));
        glVertexArrayAttribBinding(m_vao, 0, 0);

        //Color
        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 1, 4,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, color));
        glVertexArrayAttribBinding(m_vao, 1, 0);

        //Texture
        glEnableVertexArrayAttrib(m_vao, 2);
        glVertexArrayAttribFormat(m_vao, 2, 2,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, texture));
        glVertexArrayAttribBinding(m_vao, 2, 0);

        //Normal
        glEnableVertexArrayAttrib(m_vao, 3);
        glVertexArrayAttribFormat(m_vao, 3, 3,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, normal));
        glVertexArrayAttribBinding(m_vao, 3, 0);

        //Tangent
        glEnableVertexArrayAttrib(m_vao, 4);
        glVertexArrayAttribFormat(m_vao, 4, 3,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, tangent));
        glVertexArrayAttribBinding(m_vao, 4, 0);

        //Bitangent
        glEnableVertexArrayAttrib(m_vao, 5);
        glVertexArrayAttribFormat(m_vao, 5, 3,GL_FLOAT,GL_FALSE,offsetof(VertexProperties, bitangent));
        glVertexArrayAttribBinding(m_vao, 5, 0);
    }

    void GLVertexArray::attachVBO(const VertexBuffer& vbo) {
        if (vbo.getVBO() != 0) {
            glVertexArrayVertexBuffer(m_vao, 0, vbo.getVBO(), 0, sizeof(VertexProperties));
            return;
        }
        KITA_ENGINE_ERROR("[GLVertexArray] Unable to attach VertexBuffer");
    }

    void GLVertexArray::attachIBO(const IndexBuffer& ibo) {
        if (ibo.getIBO() != 0) {
            glVertexArrayElementBuffer(m_vao, ibo.getIBO());
            return;
        }
        KITA_ENGINE_ERROR("[GLVertexArray] Unable to attach IndexBuffer");
    }

    void GLVertexArray::bind() {
        KITA_ENGINE_PROFILE("Set VAO");
        glBindVertexArray(m_vao);
    }
} // Kita
