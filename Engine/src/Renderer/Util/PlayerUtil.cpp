#include "PlayerUtil.h"

#include "PhysicsUtil.h"
#include "../../Core/Engine.h"
#include "../Scene/ECS/Components/PlayerCharacterComponent.h"

namespace Kita {
    void PlayerUtil::setPosition(const Entity playerEntity, const glm::vec3 pos) {
        playerEntity.getComponent<PlayerCharacterComponent>().properties.character->SetPosition(PhysicsUtil::GLMToJPHVec3(pos));
    }
} // Kita
