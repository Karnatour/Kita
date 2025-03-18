#pragma once

namespace Kita {
    class ElementBuffer {
    public:
        virtual ~ElementBuffer() = default;
        virtual void create(std::vector<unsigned int>& indicies) = 0;
        unsigned int getEBO() const;
    protected:
        unsigned int m_ebo = 0;
    };
}
