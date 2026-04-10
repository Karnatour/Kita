#pragma once
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual void createBuffer(std::vector<unsigned int> indices) = 0;
        unsigned int getIBO() const;
        static std::unique_ptr<IndexBuffer> createPtr();
        const std::vector<unsigned int>& getIndices();
    protected:
        unsigned int m_ibo = 0;
        std::vector<unsigned int> m_indices;
    };
}
