#pragma once
#include "../Properties/VertexProperties.h"
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;
        virtual void createBuffer(std::vector<VertexProperties> vertices) = 0;
        unsigned int getVBO() const;
        static std::unique_ptr<VertexBuffer> createPtr();
        const std::vector<VertexProperties>& getVertices();
    protected:
        unsigned int m_vbo = 0;
        std::vector<VertexProperties> m_vertices;
    };
} // Kita
