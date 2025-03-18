#pragma once
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include <memory>
#include <vector>

#include "Structs/GraphicsAPI.h"

namespace Kita {
    class Mesh {
    public:
        Mesh(GraphicsAPI api, std::vector<Vertex>& vertices);

    private:
        unsigned int m_id;
        std::unique_ptr<VertexBuffer> m_vbo = VertexBuffer::create();
        std::unique_ptr<VertexArray> m_vao = VertexArray::create();
        std::unique_ptr<ElementBuffer> m_ebo = ElementBuffer::create();
    };
} // Kita
