#pragma once
#include "../../../Buffers/IndexBuffer.h"
#include <vector>

namespace Kita {
    class GLIndexBuffer final : public IndexBuffer {
    public:
        ~GLIndexBuffer() override;
        void createBuffer(const std::vector<unsigned int>& indices) override;
    };
} // Kita
