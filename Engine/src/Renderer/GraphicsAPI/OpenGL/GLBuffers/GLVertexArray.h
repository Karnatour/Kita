#pragma once
#include "../../../Buffers/VertexArray.h"

namespace Kita {
    class GLVertexArray final : public VertexArray {
    public:
        ~GLVertexArray() override;
        void createBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo) override;
        void bind() override;
    };
} // Kita
