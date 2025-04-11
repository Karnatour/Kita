#include "Mesh.h"

#include "../Core/Engine.h"

namespace Kita {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies) {
        m_id = ++m_idCounter;
        m_vao->createBuffer(vertices, indicies);
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader("../assets/shaders/DefaultVertexColor.glsl", "../assets/shaders/DefaultFragmentColor.glsl");
    }

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies, const std::string& texturePath) {
        m_id = ++m_idCounter;
        m_vao->createBuffer(vertices, indicies);
        m_shader = Engine::getEngine()->getRenderer().getShaderManager().getShader("../assets/shaders/DefaultVertexTexture.glsl", "../assets/shaders/DefaultFragmentTexture.glsl");
        m_texture = Texture::createPtr();
        auto texManager = Engine::getEngine()->getEngine()->getRenderer().getTextureManager();
        texManager.addTexture(texturePath);
        m_texture = texManager.getTexture(texturePath);
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

    void Mesh::setShader(const std::shared_ptr<Shader>& shader) {
        m_shader = shader;
    }

    std::shared_ptr<Texture> Mesh::getTexture() const {
        return m_texture;
    }

    void Mesh::setTexture(const std::shared_ptr<Texture>& texture) {
        m_texture = texture;
    }
} // Kita
