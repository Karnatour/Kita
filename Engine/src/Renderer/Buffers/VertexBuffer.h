#pragma once
#include "../Structs/Vertex.h"
#include <memory>
#include <vector>

namespace Kita {
    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void createBuffer(const std::vector<Vertex>& vertices) = 0;
        unsigned int getVBO() const;
        static std::shared_ptr<VertexBuffer> createPtr();

    protected:
        unsigned int m_vbo = 0;
    };
} // Kita
