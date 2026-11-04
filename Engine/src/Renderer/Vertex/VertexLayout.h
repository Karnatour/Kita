#pragma once
#include "VertexAttribute.h"
#include "../src/Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexLayout {
    public:
        VertexLayout(std::initializer_list<VertexAttribute> attributes);
        static VertexLayout getDefaultVertexLayout();

        const std::vector<VertexAttribute>& getVertexAttributes() const;
        size_t getStride() const;
        size_t getOffset(size_t attributeIndex) const;

    private:
        std::vector<VertexAttribute> m_attributes;
        size_t m_stride = 0;
        std::vector<size_t> m_offsets;
    };
} // Kita
