#pragma once

#include "System.h"
#include "../Entity.h"
#include "../../../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API PhysicsSystem : public System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;
    private:
        void syncTransformation(Entity entity, const glm::mat4& parentModelMatrix);
    };
} // Kita