#pragma once
#include "../../Core/DllTemplate.h"
#include <memory>

namespace Kita {
    class KITAENGINE_API ShaderStorageBuffer {
    public:
        ShaderStorageBuffer() = default;
        ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
        ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
        ShaderStorageBuffer(ShaderStorageBuffer&&) = delete;
        ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = delete;
        virtual ~ShaderStorageBuffer() = default;

        static std::unique_ptr<ShaderStorageBuffer> createPtr();

        virtual void createBuffer(size_t size, const void* data) = 0;
        virtual void bind(unsigned int position) = 0;
        virtual void upload(size_t size, const void* data) = 0;

        unsigned int getSSBO() const;

    protected:
        unsigned int m_ssbo = 0;
        size_t m_size = 0;
    };
} // Kita