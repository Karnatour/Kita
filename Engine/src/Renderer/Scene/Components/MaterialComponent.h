#pragma once
#include <memory>
#include "../Primitives/Material.h"

namespace Kita {
    struct MaterialComponent {
        std::shared_ptr<Material> material;
    };
} //Kita
