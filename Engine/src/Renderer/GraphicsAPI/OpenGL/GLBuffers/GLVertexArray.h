#pragma once
#include "../../../Buffers/VertexArray.h"

namespace Kita {
    class GLVertexArray : public VertexArray {
        void create(VertexBuffer& vbo, ElementBuffer& ibo) override;
        void bind() override;
    };
} // Kita
