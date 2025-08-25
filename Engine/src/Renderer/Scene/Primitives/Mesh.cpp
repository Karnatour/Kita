#include "../../../kitapch.h"
#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        m_vao->createBuffer(vertices, indices);
        m_materialIndex = -1;
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const int materialIndex) {
        m_vao->createBuffer(vertices, indices);
        m_materialIndex = materialIndex;
    }

    std::shared_ptr<VertexArray> Mesh::getVertexArray() const {
        return m_vao;
    }

    int Mesh::getMaterialIndex() const {
        return m_materialIndex;
    }

    void Mesh::setMaterialIndex(const int materialIndex) {
        m_materialIndex = materialIndex;
    }
} // Kita
