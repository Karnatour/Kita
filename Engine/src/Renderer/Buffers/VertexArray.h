#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <memory>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexArray {
    public:
        virtual ~VertexArray() = default;
        virtual void createBuffer(const std::vector<Vertex>& verticies, const std::vector<unsigned int>& indicies) = 0;
        virtual void bind() = 0;
        unsigned int getVAO() const;
        std::shared_ptr<VertexBuffer> getVBOobj() const;
        std::shared_ptr<IndexBuffer> getIBOobj() const;
        static std::shared_ptr<VertexArray> createPtr();

    protected:
        unsigned int m_vao = 0;
        std::shared_ptr<VertexBuffer> m_vbo = VertexBuffer::createPtr();
        std::shared_ptr<IndexBuffer> m_ibo = IndexBuffer::createPtr();
    };
} // Kita
