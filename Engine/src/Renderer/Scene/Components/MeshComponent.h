#pragma once
#include <memory>
#include "../Primitives/Mesh.h"

namespace Kita {
    struct MeshComponent {
        std::shared_ptr<Mesh> mesh;
    };
} //Kita