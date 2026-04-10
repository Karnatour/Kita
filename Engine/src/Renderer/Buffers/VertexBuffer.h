#pragma once
#include "../Vertex.h"
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void createBuffer(std::vector<Vertex> vertices) = 0;
        unsigned int getVBO() const;
        static std::unique_ptr<VertexBuffer> createPtr();
        const std::vector<Vertex>& getVertices();
    protected:
        unsigned int m_vbo = 0;
        std::vector<Vertex> m_vertices;
    };
} // Kita
