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
        virtual void upload(unsigned int size, const void* data) = 0;
        static std::unique_ptr<UniformBuffer> createPtr();
    protected:
        unsigned int m_ubo = 0;
        unsigned int m_size = 0;
    };
} // Kita
