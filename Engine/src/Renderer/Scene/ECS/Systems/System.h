#pragma once
#include "../../../../Core/DllTemplate.h"

namespace Kita {
    class Scene;

    class KITAENGINE_API System {
    public:
        struct Order {
            static constexpr int CAMERA = 0;
            static constexpr int LIGHTSHADOW = 100;
            static constexpr int GEOMETRY = 200;
            static constexpr int SKYBOX = 300;
            static constexpr int POST = 400;
        };

        virtual ~System() = default;

        virtual int getOrder() = 0;

        virtual void update(Scene& scene) = 0;

        virtual void render(Scene& scene) = 0;
    };
} // Kita
