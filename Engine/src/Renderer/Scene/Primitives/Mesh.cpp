#include "../../../kitapch.h"
#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
        m_vao->createBuffer(vertices, indices);
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const int materialIndex) {
        m_vao->createBuffer(vertices, indices);
    }

    std::shared_ptr<VertexArray> Mesh::getVertexArray() const {
        return m_vao;
    }

} // Kita
