#include "GLVertexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexBuffer::GLVertexBuffer(VertexBufferData bufferData) : VertexBuffer(std::move(bufferData)) {
        KITA_ENGINE_ASSERT(!m_bufferData.getVertices().empty(), "VertexBuffer has received empty vertices vector");

        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, m_bufferData.getVertices().size(), m_bufferData.getVertices().data(),GL_DYNAMIC_STORAGE_BIT);
    }

    GLVertexBuffer::~GLVertexBuffer() {
        if (m_vbo != 0) {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }
    }

    void GLVertexBuffer::upload(const size_t offset, const size_t size, const std::vector<std::byte>& vertices) {
        KITA_ENGINE_ASSERT(!vertices.empty(), "VertexBuffer has received empty vertices");
        glNamedBufferSubData(m_vbo, offset, size, vertices.data());
    }
} // Kita
