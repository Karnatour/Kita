#pragma once
#include "../../../Buffers/VertexBuffer.h"
#include <vector>

namespace Kita {
    class GLVertexBuffer final : public VertexBuffer {
    public:
        ~GLVertexBuffer() override;
        void createBuffer(const std::vector<Vertex>& vertices) override;
    };
} // Kita
