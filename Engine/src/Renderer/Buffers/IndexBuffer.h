#pragma once
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API IndexBuffer {
    public:
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = delete;
        IndexBuffer& operator=(IndexBuffer&&) = delete;
        virtual ~IndexBuffer() = default;

        [[nodiscard]] static std::unique_ptr<IndexBuffer> createPtr(std::vector<unsigned int> indices);

        unsigned int getIBO() const;
        const std::vector<unsigned int>& getIndices();

    protected:
        explicit IndexBuffer(std::vector<unsigned int> indices);

        unsigned int m_ibo = 0;
        std::vector<unsigned int> m_indices;
    };
}
