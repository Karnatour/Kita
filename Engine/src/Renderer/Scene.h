#pragma once
#include "Mesh.h"
#include <unordered_map>

namespace Kita {
    class Scene {
    public:
    private:
        std::unordered_map<unsigned int, Mesh> m_meshes;
    };
} // Kita
