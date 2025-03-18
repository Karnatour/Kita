#pragma once
#include "../../../Buffers/ElementBuffer.h"
#include <vector>

namespace Kita {
    class GLElementBuffer : public ElementBuffer {
    public:
        void create(std::vector<unsigned int>& indicies) override;
    };
} // Kita
