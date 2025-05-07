#pragma once
#include "../../../Buffers/UniformBuffer.h"

namespace Kita {

class GLUniformBuffer : public UniformBuffer{
public:
    ~GLUniformBuffer() override;
    void createBuffer() override;
};

} // Kita
