#pragma once
#include "Mesh.h"
#include <unordered_map>
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Scene {
    public:
    private:
        std::unordered_map<unsigned int, Mesh> m_meshes;
    };
} // Kita
