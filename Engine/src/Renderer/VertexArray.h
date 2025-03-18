#pragma once
#include "ElementBuffer.h"
#include "VertexBuffer.h"

namespace Kita {

class VertexArray {
public:
    virtual ~VertexArray() = default;
    virtual void create(VertexBuffer& vbo, ElementBuffer& ibo) = 0;
    virtual void bind() = 0;
protected:
    unsigned int m_vao = 0;
};

} // Kita
