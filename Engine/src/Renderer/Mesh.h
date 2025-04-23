#pragma once
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include <memory>
#include <vector>

#include "Textures/Texture.h"
#include "../Core/DllTemplate.h"
#include "Shaders/Shader.h"

namespace Kita {
    class KITAENGINE_API Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies, const std::string& texturePath);
        unsigned int getID() const;
        std::shared_ptr<VertexArray> getVertexArray() const;
        std::shared_ptr<Shader> getShader() const;
        void setShader(const std::shared_ptr<Shader>& shader);
        std::shared_ptr<Texture> getTexture() const;
        void setTexture(const std::shared_ptr<Texture>& texture);

    private:
        inline static unsigned int m_idCounter = 0;
        unsigned int m_id;
        std::shared_ptr<VertexArray> m_vao = VertexArray::createPtr();
        std::shared_ptr<Shader> m_shader = nullptr;
        std::shared_ptr<Texture> m_texture = nullptr;
    };
} // Kita
