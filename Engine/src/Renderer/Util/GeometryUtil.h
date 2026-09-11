#pragma once
#include "../../Core/DllTemplate.h"
#include <vector>
#include "../Properties/VertexProperties.h"

namespace Kita {
    struct MeshData {
        std::vector<VertexProperties> vertices;
        std::vector<unsigned int> indices;
    };

    struct KITAENGINE_API Geometry {
        static MeshData getCubeData();
        static MeshData getQuadData();
        static MeshData getTriangleData();
    };
}
