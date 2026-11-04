#pragma once
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API UniformBuffer {
    public:
        UniformBuffer(const UniformBuffer&) = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;
        UniformBuffer(UniformBuffer&&) = delete;
        UniformBuffer& operator=(UniformBuffer&&) = delete;
        virtual ~UniformBuffer() = default;

        [[nodiscard]] static std::unique_ptr<UniformBuffer> createPtr(size_t size, const void* data);

        virtual void bind(unsigned int position) = 0;
        virtual void upload(size_t size, const void* data) = 0;

        unsigned int getUBO() const;

    protected:
        explicit UniformBuffer(size_t size);

        unsigned int m_ubo = 0;
        size_t m_size = 0;
    };
} // Kita
