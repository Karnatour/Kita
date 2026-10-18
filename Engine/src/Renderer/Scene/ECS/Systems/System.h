#pragma once
#include "../../../../Core/DllTemplate.h"

namespace Kita {
    class Scene;

    class KITAENGINE_API System {
    public:
        struct Order {
            static constexpr int CAMERA = 0;
            static constexpr int PHYSICS = 100;
            static constexpr int LIGHTSHADOW = 200;
            static constexpr int TERRAIN = 250;
            static constexpr int GEOMETRY = 300;
            static constexpr int SKYBOX = 400;
            static constexpr int POST = 500;
        };

        explicit System(Scene& scene) : m_scene(scene) {
        }

        virtual ~System() = default;

        virtual int getOrder() = 0;

        virtual void update(Scene& scene) = 0;

        virtual void render(Scene& scene) = 0;

    protected:
        Scene& m_scene;
    };
} // Kita
