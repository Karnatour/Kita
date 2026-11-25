#include "GLVertexArray.h"
#include <glad/glad.h>

#include "../GLRendererAPI.h"

namespace Kita {
    GLVertexArray::GLVertexArray(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo) : VertexArray() {
        glCreateVertexArrays(1, &m_vao);

        if (vbo != nullptr && vbo->getVBO() != 0) {
            attachVBO(*vbo);
        }
        if (ibo != nullptr && ibo->getIBO() != 0) {
            attachIBO(*ibo);
        }
    }

    GLVertexArray::~GLVertexArray() {
        if (m_vao != 0) {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    void GLVertexArray::attachVBO(const VertexBuffer& vbo) {
        if (vbo.getVBO() != 0) {
            setupAttributes(vbo);
            return;
        }
        KITA_ENGINE_ERROR("[GLVertexArray] Unable to attach VertexBuffer, it has not beed created");
    }

    void GLVertexArray::attachIBO(const IndexBuffer& ibo) {
        if (ibo.getIBO() != 0) {
            glVertexArrayElementBuffer(m_vao, ibo.getIBO());
            return;
        }
        KITA_ENGINE_ERROR("[GLVertexArray] Unable to attach IndexBuffer, it has not been created");
    }

    void GLVertexArray::bind() {
        KITA_ENGINE_PROFILE("Set VAO");
        glBindVertexArray(m_vao);
    }

    void GLVertexArray::setupAttributes(const VertexBuffer& vbo) {
        const auto& vertexLayout = vbo.getBufferData().getVertexLayout();

        glVertexArrayVertexBuffer(m_vao, 0, vbo.getVBO(), 0, static_cast<GLsizei>(vertexLayout.getStride()));

        const auto& attributes = vertexLayout.getVertexAttributes();
        for (size_t i = 0; i < attributes.size(); ++i) {
            const auto& attribute = attributes[i];
            const auto index = static_cast<GLuint>(i);

            glEnableVertexArrayAttrib(m_vao, index);
            glVertexArrayAttribFormat(m_vao, index, static_cast<GLint>(attribute.getCount()), GLRendererAPI::convertAttributeTypeToGL(attribute.getType()),GL_FALSE, static_cast<GLuint>(vertexLayout.getOffset(i)));
            glVertexArrayAttribBinding(m_vao, index, 0);
        }
    }
} // Kita
