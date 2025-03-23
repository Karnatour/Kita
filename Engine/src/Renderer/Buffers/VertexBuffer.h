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

    protected:
        unsigned int m_vbo = 0;
    };
} // Kita
