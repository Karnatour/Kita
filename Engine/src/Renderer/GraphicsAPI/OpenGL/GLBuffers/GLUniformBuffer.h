#pragma once
#include "../../../Buffers/UniformBuffer.h"

namespace Kita {

class GLUniformBuffer final : public UniformBuffer{
public:
    ~GLUniformBuffer() override;
    void bind(unsigned int position) override;
    void upload(unsigned int size, const void* data) override;
    void createBuffer(unsigned int size, const void* data) override;
};

} // Kita
