#pragma once
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include <memory>
#include <vector>
#include "../Core/DllTemplate.h"
#include "Shaders/Shader.h"


namespace Kita {
    class KITAENGINE_API Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indicies);
        unsigned int getID() const;
        std::shared_ptr<VertexArray> getVertexArray() const;
        std::shared_ptr<Shader> getShader() const;
    private:
        unsigned int m_id;
        std::shared_ptr<VertexArray> m_vao = VertexArray::createPtr();
        std::shared_ptr<Shader> m_shader = nullptr;

    };
} // Kita
