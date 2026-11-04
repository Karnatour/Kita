#pragma once
#include <memory>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexArray {
    public:
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&&) = delete;
        VertexArray& operator=(VertexArray&&) = delete;
        virtual ~VertexArray() = default;

        [[nodiscard]] static std::unique_ptr<VertexArray> createPtr(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo);

        virtual void attachVBO(const VertexBuffer& vbo) = 0;
        virtual void attachIBO(const IndexBuffer& ibo) = 0;
        virtual void bind() = 0;

        unsigned int getVAO() const;

    protected:
        VertexArray() = default;
        unsigned int m_vao = 0;
    };
} // Kita
