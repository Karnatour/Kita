#pragma once
#include "../../Core/DllTemplate.h"
#include <vector>
#include "../Vertex/VertexBufferData.h"

namespace Kita {
    struct MeshData {
        VertexBufferData vertexBufferData;
        std::vector<unsigned int> indices;
    };

    struct KITAENGINE_API Geometry {
        static MeshData getCubeData();
        static MeshData getQuadData();
        static MeshData getTriangleData();
    };
}
