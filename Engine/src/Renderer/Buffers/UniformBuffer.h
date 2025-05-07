#pragma once
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;
        virtual void createBuffer() = 0;
        unsigned int getUBO() const;
        static std::shared_ptr<UniformBuffer> createPtr();
    protected:
        unsigned int m_ubo = 0;
    };
} // Kita
