#pragma once
#include "../../../Buffers/IndexBuffer.h"
#include <vector>

namespace Kita {
    class GLIndexBuffer final : public IndexBuffer {
    public:
        explicit GLIndexBuffer(std::vector<unsigned int> indices);
        ~GLIndexBuffer() override;
    };
} // Kita
