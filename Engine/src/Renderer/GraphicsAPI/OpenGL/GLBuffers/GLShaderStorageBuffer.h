#pragma once
#include "../../../Buffers/ShaderStorageBuffer.h"

namespace Kita {
    class GLShaderStorageBuffer final : public ShaderStorageBuffer {
    public:
        ~GLShaderStorageBuffer() override;
        void bind(unsigned position) override;
        void upload(size_t size, const void* data) override;
        void createBuffer(size_t size, const void* data) override;
    };
} // Kita