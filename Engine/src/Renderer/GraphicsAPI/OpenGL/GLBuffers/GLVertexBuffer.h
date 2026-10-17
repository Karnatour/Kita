#pragma once
#include "../../../Buffers/VertexBuffer.h"
#include <vector>

namespace Kita {
    class GLVertexBuffer final : public VertexBuffer {
    public:
        ~GLVertexBuffer() override;
        void createBuffer(std::vector<VertexProperties> vertices) override;
        void createEmptyBuffer(size_t size) override;
        void upload(size_t offset, size_t size, const std::vector<VertexProperties>& vertices) override;
    };
} // Kita
