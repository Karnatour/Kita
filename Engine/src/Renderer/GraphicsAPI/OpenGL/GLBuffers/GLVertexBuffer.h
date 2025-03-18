#pragma once
#include "../../../Buffers/VertexBuffer.h"
#include <vector>

namespace Kita {
    class GLVertexBuffer final : public VertexBuffer {
    public:
        void create(unsigned size, std::vector<Vertex>& vertices) override;
    };
} // Kita
