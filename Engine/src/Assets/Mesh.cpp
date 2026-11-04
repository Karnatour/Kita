#include "../Assets/Mesh.h"

namespace Kita {
    Mesh::Mesh(VertexBufferData bufferData, std::vector<unsigned int> indices) {
        create(std::move(bufferData), std::move(indices));
    }

    Mesh::Mesh(MeshData meshData) {
        create(std::move(meshData.vertexBufferData), std::move(meshData.indices));
    }

    void Mesh::create(VertexBufferData bufferData, std::vector<unsigned int> indices) {
        m_vbo = VertexBuffer::createPtr(std::move(bufferData));
        if (!indices.empty()) {
            m_ibo = IndexBuffer::createPtr(std::move(indices));
        }
        m_vao = m_vao->createPtr(m_vbo, m_ibo);
    }

    VertexArray& Mesh::getVertexArray() const {
        return *m_vao;
    }

    VertexBuffer& Mesh::getVertexBuffer() const {
        return *m_vbo;
    }

    IndexBuffer* Mesh::getIndexBuffer() const {
        return m_ibo.get();
    }
} // Kita
