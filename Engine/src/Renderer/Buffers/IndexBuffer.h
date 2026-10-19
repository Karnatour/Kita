#pragma once
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API IndexBuffer {
    public:
        IndexBuffer() = default;
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = delete;
        IndexBuffer& operator=(IndexBuffer&&) = delete;
        virtual ~IndexBuffer() = default;

        [[nodiscard]] static std::unique_ptr<IndexBuffer> createPtr();

        virtual void createBuffer(std::vector<unsigned int> indices) = 0;

        unsigned int getIBO() const;
        const std::vector<unsigned int>& getIndices();

    protected:
        unsigned int m_ibo = 0;
        std::vector<unsigned int> m_indices;
    };
}
