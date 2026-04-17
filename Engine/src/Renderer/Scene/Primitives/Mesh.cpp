#include "../../../kitapch.h"
#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(std::vector<VertexProperties> vertices, std::vector<unsigned int> indices) {
        create(std::move(vertices), std::move(indices));
    }

    Mesh::Mesh(Geometry::MeshData data) {
        create(std::move(data.vertices), std::move(data.indices));
    }

    void Mesh::create(std::vector<VertexProperties> vertices, std::vector<unsigned int> indices) {
        m_vbo->createBuffer(std::move(vertices));
        if (!indices.empty()) {
            m_ibo = IndexBuffer::createPtr();
            m_ibo->createBuffer(std::move(indices));
        }
        m_vao->createBuffer(m_vbo, m_ibo);
    }

    const VertexArray& Mesh::getVertexArray() const {
        return *m_vao;
    }

    const VertexBuffer& Mesh::getVertexBuffer() const {
        return *m_vbo;
    }

    const IndexBuffer* Mesh::getIndexBuffer() const {
        return m_ibo.get();
    }
} // Kita
