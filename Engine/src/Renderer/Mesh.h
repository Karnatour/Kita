#pragma once
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include <memory>
#include <vector>
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices,const std::vector<unsigned int>& indicies);
        unsigned int getID() const;
    private:
        unsigned int m_id;
        size_t m_verticesCount;
        std::shared_ptr<VertexBuffer> m_vbo = VertexBuffer::createPtr();
        std::shared_ptr<VertexArray> m_vao = VertexArray::createPtr();
        std::shared_ptr<IndexBuffer> m_ibo = IndexBuffer::createPtr();
    };
} // Kita
