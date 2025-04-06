#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indicies) {
        static int idCounter = 0;
        m_id = ++idCounter;
        m_vao->createBuffer(vertices,indicies);
    }

    unsigned int Mesh::getID() const {
        return m_id;
    }

    std::shared_ptr<VertexArray> Mesh::getVertexArray() const {
        return m_vao;
    }
} // Kita