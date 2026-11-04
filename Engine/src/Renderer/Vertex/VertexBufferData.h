#pragma once
#include "../src/Core/DllTemplate.h"
#include "VertexLayout.h"
#include "../src/Core/Assert.h"

namespace Kita {
    class KITAENGINE_API VertexBufferData {
    public:
        VertexBufferData(std::vector<std::byte> vertices, VertexLayout layout);
        const std::vector<std::byte>& getVertices() const;
        const VertexLayout& getVertexLayout() const;
        size_t getVertexCount() const;

        template <typename T>
        static void appendBytes(std::vector<std::byte>& vertices, const T& value) {
            static_assert(std::is_trivially_copyable_v<T>);
            const auto* bytes = reinterpret_cast<const std::byte*>(&value);
            vertices.insert(vertices.end(), bytes, bytes + sizeof(T));
        }

    private:
        std::vector<std::byte> m_vertices;
        VertexLayout m_vertexLayout;

    public:
        template <typename T>
        T getAttribute(const size_t vertexIndex, const size_t attributeIndex) const {
            static_assert(std::is_trivially_copyable_v<T>);
            KITA_ENGINE_ASSERT(sizeof(T) == m_vertexLayout.getVertexAttributes().at(attributeIndex).getTotalSize(), std::format("T {} doesn't match attribute index",typeid(T).name()));

            const size_t offset = vertexIndex * m_vertexLayout.getStride() + m_vertexLayout.getOffset(attributeIndex);

            T result;
            std::memcpy(&result, m_vertices.data() + offset, sizeof(T));
            return result;
        }

        template <typename T>
        std::vector<T> getAttributes(const size_t attributeIndex) const {
            static_assert(std::is_trivially_copyable_v<T>);
            KITA_ENGINE_ASSERT(sizeof(T) == m_vertexLayout.getVertexAttributes().at(attributeIndex).getTotalSize(), std::format("T {} doesn't match attribute index",typeid(T).name()));

            const size_t stride = m_vertexLayout.getStride();
            const size_t offset = m_vertexLayout.getOffset(attributeIndex);
            const size_t count = m_vertices.size() / stride;

            std::vector<T> result(count);
            for (size_t i = 0; i < count; ++i) {
                std::memcpy(&result[i], m_vertices.data() + i * stride + offset, sizeof(T));
            }
            return result;
        }
    };
} // Kita
