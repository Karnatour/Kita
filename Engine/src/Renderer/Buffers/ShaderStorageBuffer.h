#pragma once
#include "../../Core/DllTemplate.h"
#include <memory>

namespace Kita {
    class KITAENGINE_API ShaderStorageBuffer {
    public:
        virtual ~ShaderStorageBuffer() = default;
        virtual void createBuffer(unsigned int size, const void* data) = 0;
        unsigned int getSSBO() const;
        virtual void bind(unsigned int position) = 0;
        virtual void upload(unsigned int size, const void* data) = 0;
        static std::unique_ptr<ShaderStorageBuffer> createPtr();
    protected:
        unsigned int m_ssbo = 0;
        unsigned int m_size = 0;
    };
} // Kita