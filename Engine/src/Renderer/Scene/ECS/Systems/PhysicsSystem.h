#pragma once
#include <array>
#include <glm/glm.hpp>
#include <Jolt/Jolt.h>
#include <Jolt/Core/Reference.h>

#include "System.h"
#include "../Entity.h"
#include "../../../../Core/DllTemplate.h"
#include "../../../../Events/PhysicsManagerEvents.h"

namespace JPH {
    class WheelSettingsWV;
}

namespace Kita {
    struct VehicleProperties;

    class KITAENGINE_API PhysicsSystem : public System {
    public:
        explicit PhysicsSystem(Scene& scene);
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        //Player
        void initPlayerCharacterComponents();
        void preUpdatePlayerCharacter(const PreUpdateEvent& event);
        void handlePlayerCharacterInput();

        //Vehicle
        void initVehicleComponents();
        JPH::Ref<JPH::WheelSettingsWV> makeWheel(entt::entity wheelEnttID, bool isFront, bool isLeft, const VehicleProperties& p) const;
        glm::vec3 getWheelCenter(const VehicleProperties& p);
        std::array<JPH::Ref<JPH::WheelSettingsWV>, 4> getVehicleWheels(VehicleProperties& p);
        std::pair<glm::vec3, glm::vec3> getWheelMinMaxPos(entt::entity wheelEnttID) const;
        void preUpdateVehicle(const PreUpdateEvent& event);
        void handleVehicleInput();

        void updateModelMatrices();
        void syncTransformation(Entity entity, const glm::mat4& parentModelMatrix);

        // We need to store scene because of PhysicsManagerEvents
        Scene& m_scene;
    };
} // Kita
