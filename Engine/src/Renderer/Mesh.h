#pragma once
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include <memory>
#include <vector>

#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, int materialIndex);
        unsigned int getID() const;
        std::shared_ptr<VertexArray> getVertexArray() const;
        int getMaterialIndex() const;
        void setMaterialIndex(int materialIndex);

    private:
        std::shared_ptr<VertexArray> m_vao = VertexArray::createPtr();
        int m_materialIndex;
    };
} // Kita
