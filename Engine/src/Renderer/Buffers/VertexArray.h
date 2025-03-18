#pragma once
#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include <memory>

namespace Kita {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void create(VertexBuffer& vbo, ElementBuffer& ibo) = 0;
        virtual void bind() = 0;
        unsigned int getVAO() const;
        static std::unique_ptr<VertexArray> create();

    protected:
        unsigned int m_vao = 0;
    };
} // Kita
