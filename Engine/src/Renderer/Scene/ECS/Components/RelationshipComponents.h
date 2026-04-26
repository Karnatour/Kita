#pragma once
#include <entt/entity/entity.hpp>
#include <vector>

namespace Kita {
    // Since entt stores each component in it's own pool having vector as component won't hurt us if we won't use it in hotpath -> rendering
    struct ChildrenComponent {
        std::vector<entt::entity> children;
    };

} //Kita