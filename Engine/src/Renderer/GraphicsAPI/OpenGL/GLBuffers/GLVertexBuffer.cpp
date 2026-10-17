#include "GLVertexBuffer.h"

#include <glad/glad.h>

namespace Kita {
    GLVertexBuffer::~GLVertexBuffer() {
        if (m_vbo != 0) {
            glDeleteBuffers(1, &m_vbo);
            m_vbo = 0;
        }
    }

    void GLVertexBuffer::createBuffer(std::vector<VertexProperties> vertices) {
        if (vertices.empty()) {
            return;
        }
        m_vertices = std::move(vertices);
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, static_cast<GLsizeiptr>(sizeof(VertexProperties) * m_vertices.size()), m_vertices.data(),GL_DYNAMIC_STORAGE_BIT);
    }

    void GLVertexBuffer::createEmptyBuffer(const size_t size) {
        KITA_ENGINE_ASSERT(m_vbo == 0, "Failed to create empty buffer since VertexBuffer was already created");
        glCreateBuffers(1, &m_vbo);
        glNamedBufferStorage(m_vbo, size, nullptr,GL_DYNAMIC_STORAGE_BIT);
    }

    void GLVertexBuffer::upload(const size_t offset, const size_t size, const std::vector<VertexProperties>& vertices) {
        KITA_ENGINE_ASSERT(m_vbo != 0, "VertexBuffer has not been created, call createBuffer first");
        KITA_ENGINE_ASSERT(!vertices.empty(), "VertexBuffer has received empty vertices");
        glNamedBufferSubData(m_vbo, offset, size, vertices.data());
    }
} // Kita
