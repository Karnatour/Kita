#pragma once
#include <vector>
#include "Vertex.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API Cube {
        static std::vector<Vertex> vertices;

        static std::vector<unsigned int> indices;
    };
}
