#pragma once
#include "../../../Buffers/VertexBuffer.h"
#include <vector>

namespace Kita {
    class GLVertexBuffer final : public VertexBuffer {
    public:
        explicit GLVertexBuffer(VertexBufferData bufferData);
        ~GLVertexBuffer() override;
        void upload(size_t offset, size_t size, const std::vector<std::byte>& vertices) override;
    };
} // Kita
