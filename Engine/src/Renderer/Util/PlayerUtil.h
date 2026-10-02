#pragma once
#include <glm/glm.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class Entity;

    class KITAENGINE_API PlayerUtil {
    public:
        static void setPosition(Entity playerEntity, glm::vec3 pos);
    };
} // Kita
