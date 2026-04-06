#pragma once
#include <vector>
#include "Vertex.h"

namespace Kita {
    struct Cube {
        static std::vector<Vertex> vertices;

        static std::vector<unsigned int> indices;
    };

    struct Quad {
        static std::vector<Vertex> vertices;

        static std::vector<unsigned int> indices;
    };
}
