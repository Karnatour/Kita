#pragma once
#include "../Renderer/Buffers/VertexArray.h"
#include "../Renderer/Buffers/VertexBuffer.h"
#include "../Renderer/Buffers/IndexBuffer.h"
#include <memory>
#include <vector>

#include "Asset.h"
#include "../Core/DllTemplate.h"
#include "../Renderer/Util/GeometryUtil.h"
#include "../Renderer/Vertex/VertexBufferData.h"

namespace Kita {
    class KITAENGINE_API Mesh : public Asset {
    public:
        Mesh(VertexBufferData bufferData, std::vector<unsigned int> indices);
        explicit Mesh(MeshData meshData);
        VertexArray& getVertexArray() const;
        VertexBuffer& getVertexBuffer() const;
        IndexBuffer* getIndexBuffer() const;

    private:
        void create(VertexBufferData bufferData, std::vector<unsigned int> indices);
        std::unique_ptr<VertexArray> m_vao;
        std::unique_ptr<VertexBuffer> m_vbo;
        std::unique_ptr<IndexBuffer> m_ibo;
    };
} // Kita
