#pragma once
#include "../../../../Core/DllTemplate.h"

namespace Kita {
    class Scene;

    class KITAENGINE_API System {
    public:
        virtual ~System() = default;

        virtual void update(Scene& scene) {
        };

        virtual void render(Scene& scene) {
        };
    };
} // Kita
