#include "../../../kitapch.h"
#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
        m_vbo->createBuffer(std::move(vertices));
        if (!indices.empty()) {
            m_ibo = IndexBuffer::createPtr();
            m_ibo->createBuffer(std::move(indices));
        }
        m_vao->createBuffer(m_vbo, m_ibo);
    }

    const std::unique_ptr<VertexArray>& Mesh::getVertexArray() const {
        return m_vao;
    }

    const std::unique_ptr<VertexBuffer>& Mesh::getVertexBuffer() const {
        return m_vbo;
    }

    const std::unique_ptr<IndexBuffer>& Mesh::getIndexBuffer() const {
        return m_ibo;
    }
} // Kita
