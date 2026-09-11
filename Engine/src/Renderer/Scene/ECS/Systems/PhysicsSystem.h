#pragma once

#include "System.h"
#include "../Entity.h"
#include "../../../../Core/DllTemplate.h"
#include "../../../../Events/PhysicsManagerEvents.h"

namespace Kita {
    class KITAENGINE_API PhysicsSystem : public System {
    public:
        explicit PhysicsSystem(Scene& scene);
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        void initPlayerCharacterComponents();
        void handlePlayerCharacterInput();
        void preUpdatePlayerCharacter(const PreUpdateEvent& event);
        void updateModelMatrices();
        void syncTransformation(Entity entity, const glm::mat4& parentModelMatrix);

        // We need to store scene because of PhysicsManagerEvents
        Scene& m_scene;
    };
} // Kita
