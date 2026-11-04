#pragma once
#include "../../Core/DllTemplate.h"
#include <memory>

namespace Kita {
    class KITAENGINE_API ShaderStorageBuffer {
    public:
        ShaderStorageBuffer(const ShaderStorageBuffer&) = delete;
        ShaderStorageBuffer& operator=(const ShaderStorageBuffer&) = delete;
        ShaderStorageBuffer(ShaderStorageBuffer&&) = delete;
        ShaderStorageBuffer& operator=(ShaderStorageBuffer&&) = delete;
        virtual ~ShaderStorageBuffer() = default;

        [[nodiscard]] static std::unique_ptr<ShaderStorageBuffer> createPtr(size_t size, const void* data);

        virtual void bind(unsigned int position) = 0;
        virtual void upload(size_t size, const void* data) = 0;

        unsigned int getSSBO() const;

    protected:
        explicit ShaderStorageBuffer(size_t size);

        unsigned int m_ssbo = 0;
        size_t m_size = 0;
    };
} // Kita