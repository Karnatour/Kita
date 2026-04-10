#pragma once
#include "../../../Buffers/VertexArray.h"

namespace Kita {
    class GLVertexArray final : public VertexArray {
    public:
        ~GLVertexArray() override;
        void createBuffer(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo) override;
        void bind() override;
    };
} // Kita
