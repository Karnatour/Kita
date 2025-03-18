#pragma once
#include "../../VertexArray.h"

namespace Kita {

class GLVertexArray : public VertexArray{
    void create(VertexBuffer& vbo, ElementBuffer& ibo) override;
    void bind() override;
};

} // Kita
