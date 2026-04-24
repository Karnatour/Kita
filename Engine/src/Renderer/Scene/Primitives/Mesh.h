#pragma once
#include "../../Buffers/VertexArray.h"
#include "../../Buffers/VertexBuffer.h"
#include "../../Buffers/IndexBuffer.h"
#include <memory>
#include <vector>

#include "../../../Assets/Asset.h"
#include "../../../Core/DllTemplate.h"
#include "../../Util/GeometryUtil.h"

namespace Kita {
    class KITAENGINE_API Mesh : public Asset {
    public:
        Mesh(std::vector<VertexProperties> vertices, std::vector<unsigned int> indices);
        explicit Mesh(Geometry::MeshData data);
        VertexArray& getVertexArray() const;
        VertexBuffer& getVertexBuffer() const;
        IndexBuffer* getIndexBuffer() const;

    private:
        void create(std::vector<VertexProperties> vertices, std::vector<unsigned int> indices);
        std::unique_ptr<VertexArray> m_vao = VertexArray::createPtr();
        std::unique_ptr<VertexBuffer> m_vbo = VertexBuffer::createPtr();
        std::unique_ptr<IndexBuffer> m_ibo;
    };
} // Kita
