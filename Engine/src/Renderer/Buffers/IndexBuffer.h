#pragma once
#include <memory>
#include <vector>

namespace Kita {
    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;
        virtual void createBuffer(const std::vector<unsigned int>& indicies) = 0;
        unsigned int getEBO() const;
        static std::shared_ptr<IndexBuffer> createPtr();
    protected:
        unsigned int m_ebo = 0;
    };
}
