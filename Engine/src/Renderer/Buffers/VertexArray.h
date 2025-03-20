#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void createBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo) = 0;
        virtual void bind() = 0;
        unsigned int getVAO() const;
        static std::shared_ptr<VertexArray> createPtr();

    protected:
        unsigned int m_vao = 0;
    };
} // Kita
