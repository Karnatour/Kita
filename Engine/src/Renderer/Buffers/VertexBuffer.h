#pragma once
#include "../Structs/Vertex.h"
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void createBuffer(const std::vector<Vertex>& vertices) = 0;
        unsigned int getVBO() const;
        static std::shared_ptr<VertexBuffer> createPtr();
        size_t getVerticiesCount() const;
        std::vector<Vertex>& getVertices();
    protected:
        unsigned int m_vbo = 0;
        size_t m_verticiesCount = 0;
        std::vector<Vertex> m_vertices;
    };
} // Kita
