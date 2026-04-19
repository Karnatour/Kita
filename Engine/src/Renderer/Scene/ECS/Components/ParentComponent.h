#pragma once
#include <entt/entity/entity.hpp>

namespace Kita {
    struct ParentComponent {
        entt::entity parent = entt::null;
    };
} //Kita