#pragma once
#include <memory>
#include <vector>
#include "../../Core/DllTemplate.h"
#include "../Vertex/VertexBufferData.h"

namespace Kita {

    class KITAENGINE_API VertexBuffer {
    public:
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;
        virtual ~VertexBuffer() = default;

        [[nodiscard]] static std::unique_ptr<VertexBuffer> createPtr(VertexBufferData bufferData);

        virtual void upload(size_t offset, size_t size, const std::vector<std::byte>& vertices) = 0;

        unsigned int getVBO() const;
        const VertexBufferData& getBufferData() const;

    protected:
        explicit VertexBuffer(VertexBufferData bufferData);

        unsigned int m_vbo = 0;
        VertexBufferData m_bufferData;
    };
} // Kita
