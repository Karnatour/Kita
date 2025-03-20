#include "Mesh.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indicies) {
        static int idCounter = 0;
        m_id = ++idCounter;
        m_vbo->createBuffer(vertices);
        m_vao->createBuffer(*m_vbo,*m_ibo);
        m_ibo->createBuffer(indicies);
    }
} // Kita