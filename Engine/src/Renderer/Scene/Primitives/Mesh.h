#pragma once
#include "../../Buffers/VertexArray.h"
#include "../../Buffers/VertexBuffer.h"
#include "../../Buffers/IndexBuffer.h"
#include <memory>
#include <vector>

#include "../../../Assets/Asset.h"
#include "../../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Mesh : public Asset {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        const std::unique_ptr<VertexArray>& getVertexArray() const;
        const std::unique_ptr<VertexBuffer>& getVertexBuffer() const;
        const std::unique_ptr<IndexBuffer>& getIndexBuffer() const;

    private:
        std::unique_ptr<VertexArray> m_vao = VertexArray::createPtr();
        std::unique_ptr<VertexBuffer> m_vbo = VertexBuffer::createPtr();
        std::unique_ptr<IndexBuffer> m_ibo;
    };
} // Kita
