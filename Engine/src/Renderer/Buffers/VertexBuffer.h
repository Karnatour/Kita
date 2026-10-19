#pragma once
#include "../Properties/VertexProperties.h"
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexBuffer {
    public:
        VertexBuffer() = default;
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;
        virtual ~VertexBuffer() = default;

        [[nodiscard]] static std::unique_ptr<VertexBuffer> createPtr();

        virtual void createBuffer(std::vector<VertexProperties> vertices) = 0;
        virtual void createEmptyBuffer(size_t size) = 0;
        virtual void upload(size_t offset, size_t size, const std::vector<VertexProperties>& vertices) = 0;

        unsigned int getVBO() const;
        const std::vector<VertexProperties>& getVertices();

    protected:
        unsigned int m_vbo = 0;
        std::vector<VertexProperties> m_vertices;
    };
} // Kita
