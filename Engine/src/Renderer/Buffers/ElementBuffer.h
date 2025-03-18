#pragma once
#include <memory>
#include <vector>

namespace Kita {
    class ElementBuffer {
    public:
        virtual ~ElementBuffer() = default;
        virtual void create(std::vector<unsigned int>& indicies) = 0;
        unsigned int getEBO() const;
        static std::unique_ptr<ElementBuffer> create();
    protected:
        unsigned int m_ebo = 0;
    };
}
