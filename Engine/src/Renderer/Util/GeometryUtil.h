#pragma once
#include <vector>
#include "../Properties/VertexProperties.h"

namespace Kita::Geometry {
    struct MeshData {
        std::vector<VertexProperties> vertices;
        std::vector<unsigned int> indices;
    };

    MeshData getCubeData();
    MeshData getQuadData();
}
