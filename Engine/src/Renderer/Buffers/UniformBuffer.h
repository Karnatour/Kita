#pragma once
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API UniformBuffer {
    public:
        virtual ~UniformBuffer() = default;
        virtual void createBuffer(unsigned int size, const void* data) = 0;
        unsigned int getUBO() const;
        virtual void bind(unsigned int position) = 0;
        virtual void update(unsigned int size, const void* data) = 0;
        static std::shared_ptr<UniformBuffer> createPtr();
    protected:
        unsigned int m_ubo = 0;
    };
} // Kita
