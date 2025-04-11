#include "Mesh.h"

#include "../Core/Engine.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies) {
        m_id = ++m_idCounter;
        m_vao->createBuffer(vertices, indicies);
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader("../assets/shaders/DefaultVertex.glsl", "../assets/shaders/DefaultFragment.glsl");
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies, const std::string& texturePath) {
        m_id = ++m_idCounter;
        m_vao->createBuffer(vertices, indicies);
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader("../assets/shaders/DefaultVertex.glsl", "../assets/shaders/DefaultFragment.glsl");
        m_texture = Texture::createPtr();
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
