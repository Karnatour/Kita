#pragma once

#include <optional>
#include <Jolt/Physics/EActivation.h>

#include "Entity.h"
#include "../../../Assets/AssetManager.h"
#include "../../../Core/DllTemplate.h"
#include "../../../Physics/PhysicsLayers.h"
#include "Components/TransformationComponent.h"

namespace Kita {
    class AssetManager;

    struct KITAENGINE_API EntityBuilder {
        static Entity createDirectionalLight(Scene& scene, bool castShadows = true);
        static Entity createPlayerCharacter(Scene& scene, const std::optional<std::string>& name, glm::vec3 position = glm::vec3(0.0f));
        static Entity createActiveCamera(Scene& scene);
        static Entity createRootRenderEntity(Scene& scene, const std::optional<std::string>& name, const std::optional<TransformationComponent>& transformationComponent = std::nullopt);
        static Entity createNodeRenderEntity(Scene& scene, const std::optional<std::string>& name, const std::optional<TransformationComponent>& transformationComponent = std::nullopt);
        static Entity createChildRenderEntity(Scene& scene, AssetManager::AssetID meshAssetID, AssetManager::AssetID shaderAssetID);
        static bool finalizeStaticBody(Entity entity, PhysicsLayers::Layers layers, JPH::EActivation activate);
    };
} // Kita
