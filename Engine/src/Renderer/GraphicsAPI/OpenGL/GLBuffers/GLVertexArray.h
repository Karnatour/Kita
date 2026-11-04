#pragma once
#include "../../../Buffers/VertexArray.h"

namespace Kita {
    class GLVertexArray final : public VertexArray {
    public:
        GLVertexArray(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo);
        ~GLVertexArray() override;
        void attachVBO(const VertexBuffer& vbo) override;
        void attachIBO(const IndexBuffer& ibo) override;
        void bind() override;
    private:
        void setupAttributes(const VertexBuffer& vbo);
    };
} // Kita
