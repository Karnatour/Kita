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
    protected:
        unsigned int m_ibo = 0;
    };
}
