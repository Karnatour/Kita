#pragma once
#include "../../../Buffers/VertexArray.h"

namespace Kita {
    class GLVertexArray final : public VertexArray {
    public:
        ~GLVertexArray() override;
        void createBuffer(const std::vector<Vertex>& verticies, const std::vector<unsigned int>& indices) override;
        void bind() override;
    };
} // Kita
