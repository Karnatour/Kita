#pragma once
#include "../../ElementBuffer.h"

namespace Kita {

class GLElementBuffer : public ElementBuffer {
public:
    void create(std::vector<unsigned int>& indicies) override;
};

} // Kita
