#pragma once
#include "../../../Buffers/ShaderStorageBuffer.h"

namespace Kita {
    class GLShaderStorageBuffer final : public ShaderStorageBuffer {
    public:
        ~GLShaderStorageBuffer() override;
        void bind(unsigned position) override;
        void update(unsigned size, const void* data) override;
        void createBuffer(unsigned size, const void* data) override;
    };
} // Kita