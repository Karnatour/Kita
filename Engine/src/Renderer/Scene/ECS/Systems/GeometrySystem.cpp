#include "../../../../kitapch.h"
#include "GeometrySystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/MaterialComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/RenderTags.h"
#include "../Components/TransformationComponent.h"

namespace Kita {
    int GeometrySystem::getOrder() {
        return Order::GEOMETRY;
    }

    void GeometrySystem::update(Scene& scene) {
    }

    void GeometrySystem::render(Scene& scene) {
        const auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        renderer.getMainFramebuffer().bind();

        for (const auto& [entityID, meshComp,shaderComp,transformationComp] : scene.view<MeshComponent, ShaderComponent, TransformationComponent, RenderInMainPass>().each()) {
            const auto entity = Entity(&scene, entityID);
            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComp.meshID), assetManager.getAsset<Shader>(shaderComp.shaderID), transformationComp.model, fetchTextures(assetManager, entity));
        }

        renderer.getMainFramebuffer().unbind();
    }

    std::array<Texture*, 3> GeometrySystem::fetchTextures(const AssetManager& assetManager, Entity entity) {
        std::array<Texture*, 3> textures = {};

        if (const auto* diffuseMap = entity.tryGetComponent<DiffuseMapComponent>(); diffuseMap) {
            textures[0] = &assetManager.getAsset<Texture>(diffuseMap->textureID);
        }

        if (const auto* specularMap = entity.tryGetComponent<SpecularMapComponent>(); specularMap) {
            textures[1] = &assetManager.getAsset<Texture>(specularMap->textureID);
        }

        if (const auto* normalMap = entity.tryGetComponent<NormalMapComponent>(); normalMap) {
            textures[2] = &assetManager.getAsset<Texture>(normalMap->textureID);
        }

        return textures;
    }
} // Kita
