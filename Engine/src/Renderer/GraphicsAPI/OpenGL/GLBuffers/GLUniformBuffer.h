#pragma once
#include "../../../Buffers/UniformBuffer.h"

namespace Kita {

class GLUniformBuffer final : public UniformBuffer{
public:
    GLUniformBuffer(size_t size, const void* data);
    ~GLUniformBuffer() override;
    void bind(unsigned int position) override;
    void upload(size_t size, const void* data) override;
};

} // Kita
