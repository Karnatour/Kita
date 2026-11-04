#include "VertexBufferData.h"

namespace Kita {
    VertexBufferData::VertexBufferData(std::vector<std::byte> vertices, VertexLayout layout) : m_vertices(std::move(vertices)), m_vertexLayout(std::move(layout)) {
    }

    const std::vector<std::byte>& VertexBufferData::getVertices() const {
        return m_vertices;
    }

    const VertexLayout& VertexBufferData::getVertexLayout() const {
        return m_vertexLayout;
    }

    size_t VertexBufferData::getVertexCount() const {
        return m_vertices.size() / m_vertexLayout.getStride();
    }
} // Kita
