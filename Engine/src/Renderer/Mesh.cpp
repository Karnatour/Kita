#include "Mesh.h"

#include "../Core/Engine.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies) {
        static int idCounter = 0;
        m_id = ++idCounter;
        m_vao->createBuffer(vertices, indicies);
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader("../assets/shaders/DefaultVertex.glsl", "../assets/shaders/DefaultFragment.glsl");
    }

    unsigned int Mesh::getID() const {
        return m_id;
    }

    std::shared_ptr<VertexArray> Mesh::getVertexArray() const {
        return m_vao;
    }

    std::shared_ptr<Shader> Mesh::getShader() const {
        return m_shader;
    }
} // Kita
