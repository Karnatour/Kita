#pragma once
#include "Structs/Vertex.h"

namespace Kita {

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void create(unsigned int size, std::vector<Vertex>& vertices) = 0;
        unsigned int getVBO() const;
    protected:
        unsigned int m_vbo = 0;
    };
} // Kita
