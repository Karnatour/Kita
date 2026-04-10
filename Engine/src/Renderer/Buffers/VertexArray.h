#pragma once
#include <memory>

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void createBuffer(const std::unique_ptr<VertexBuffer>& vbo, const std::unique_ptr<IndexBuffer>& ibo) = 0;
        virtual void bind() = 0;
        unsigned int getVAO() const;
        static std::unique_ptr<VertexArray> createPtr();

    protected:
        unsigned int m_vao = 0;
    };
} // Kita
