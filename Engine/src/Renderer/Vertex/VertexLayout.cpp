#include "VertexLayout.h"

namespace Kita {
    VertexLayout::VertexLayout(const std::initializer_list<VertexAttribute> attributes) : m_attributes(attributes) {
        size_t totalStride = 0;
        for (auto attribute : m_attributes) {
            m_offsets.push_back(m_stride);
            totalStride = totalStride + static_cast<int>(attribute.getCount()) * attribute.getTypeSize();
        }
        m_stride = totalStride;
    }

    /*
        struct VertexProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec2 texture = glm::vec2(0.0f, 0.0f);
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
    */
    VertexLayout VertexLayout::getDefaultVertexLayout() {
        return {
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE},
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::FOUR},
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::TWO},
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE},
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE},
            {VertexAttribute::Type::FLOAT, VertexAttribute::Count::THREE},
        };
    }

    const std::vector<VertexAttribute>& VertexLayout::getVertexAttributes() const {
        return m_attributes;
    }

    size_t VertexLayout::getStride() const {
        KITA_ENGINE_ASSERT(m_stride == 0, "Stride can't be zero");
        return m_stride;
    }

    size_t VertexLayout::getOffset(const size_t attributeIndex) const {
        return m_offsets.at(attributeIndex);
    }
} // Kita
