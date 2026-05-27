#include "../../../../kitapch.h"
#include "GeometrySystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/MaterialComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/RenderTags.h"
#include "../Components/TransformationComponent.h"
#include "../Components/LightShadowComponents.h"
#include "../Components/SceneComponent.h"
#include "../Components/SkyboxComponent.h"

namespace Kita {
    struct DirectionalShadowProperties;

    int GeometrySystem::getOrder() {
        return Order::GEOMETRY;
    }

    void GeometrySystem::update(Scene& scene) {
    }

    void GeometrySystem::render(Scene& scene) {
        const auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        renderer.getMainFramebuffer().bind();

        for (const auto& [entityID, meshComp,materialCmp,transformationComp] : scene.view<MeshComponent, MaterialComponent, TransformationComponent, RenderInMainPass>().each()) {
            auto& shader = assetManager.getAsset<Shader>(materialCmp.shaderID);
            shader.bind();
            shader.setUniformFloat("iblIntensity", Entity(&scene, scene.view<SceneComponent>().front()).getComponent<SceneComponent>().properties.iblIntensity); //TODO Move to UBO ?

            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComp.meshID), shader, transformationComp.model,
                                fetchTextures(assetManager, materialCmp, scene));
        }

        renderer.getMainFramebuffer().unbind();
    }

    std::array<Texture*, 5> GeometrySystem::fetchTextures(const AssetManager& assetManager, const MaterialComponent& materialCmp, Scene& scene) {
        std::array<Texture*, 5> textures = {};

        if (materialCmp.albedoTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[0] = &assetManager.getAsset<Texture>(materialCmp.albedoTextureID);
        }

        if (materialCmp.metallicRoughnessTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[1] = &assetManager.getAsset<Texture>(materialCmp.metallicRoughnessTextureID);
        }

        if (materialCmp.normalTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[2] = &assetManager.getAsset<Texture>(materialCmp.normalTextureID);
        }

        if (Entity skyboxEntity(&scene, scene.view<SkyboxComponent>().front()); skyboxEntity) {
            if (const auto& skyboxCmp = skyboxEntity.getComponent<SkyboxComponent>(); skyboxCmp.irradianceCubemapID != AssetManager::INVALID_ASSET_ID) {
                textures[3] = &assetManager.getAsset<Texture>(skyboxCmp.irradianceCubemapID);
            }
        }

        if (Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front()); dirShadowEntity) {
            textures[4] = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties.texture;
        }

        return textures;
    }
} // Kita
