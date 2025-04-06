#pragma once
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual void createBuffer(const std::vector<unsigned int>& indicies) = 0;
        unsigned int getIBO() const;
        static std::shared_ptr<IndexBuffer> createPtr();
        size_t getIndicesCount() const;
    protected:
        unsigned int m_ibo = 0;
        size_t m_indicesCount = 0;
    };
}
