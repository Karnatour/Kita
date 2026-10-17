#pragma once
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API UniformBuffer {
    public:
        UniformBuffer() = default;
        UniformBuffer(const UniformBuffer&) = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;
        UniformBuffer(UniformBuffer&&) = delete;
        UniformBuffer& operator=(UniformBuffer&&) = delete;
        virtual ~UniformBuffer() = default;

        static std::unique_ptr<UniformBuffer> createPtr();

        virtual void createBuffer(unsigned int size, const void* data) = 0;
        virtual void bind(unsigned int position) = 0;
        virtual void upload(unsigned int size, const void* data) = 0;

        unsigned int getUBO() const;

    protected:
        unsigned int m_ubo = 0;
        unsigned int m_size = 0;
    };
} // Kita
