#include "GeometrySystem.h"

#include "../../Scene.h"
#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../Components/Components.h"

namespace Kita {
    struct DirectionalShadowProperties;

    int GeometrySystem::getOrder() {
        return Order::GEOMETRY;
    }

    void GeometrySystem::update([[maybe_unused]] Scene& scene) {
    }

    void GeometrySystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("Geometry system render");
        const auto debugScope = DebugScope::createPtr();
        debugScope->scope("Geometry pass");

        const auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        renderer.getMainFramebuffer().bind();

        for (const auto& [entity, children,transformation] : scene.view<ChildrenComponent, TransformationComponent, RenderInMainPass>().each()) {
            for (const auto child : children.children) {
                if (Entity childEntity(&scene, child); childEntity.hasAllComponents<MeshComponent, MaterialComponent>()) {
                    Mesh& mesh = assetManager.getAsset<Mesh>(childEntity.getComponent<MeshComponent>().meshID);

                    auto material = childEntity.getComponent<MaterialComponent>();
                    Shader& shader = assetManager.getAsset<Shader>(material.shaderID);

                    shader.bind();
                    shader.setUniformFloat("iblIntensity", scene.contextGet<SceneSettingsComponent>()->properties.iblIntensity); //TODO Move to UBO ?

                    renderer.renderMesh(mesh, shader, transformation.worldModel, fetchTextures(assetManager, material, scene));
                }
            }
        }

        renderer.getMainFramebuffer().unbind();
    }

    std::array<Texture*, 9> GeometrySystem::fetchTextures(const AssetManager& assetManager, const MaterialComponent& materialCmp, Scene& scene) {
        KITA_ENGINE_PROFILE("Texture fetch");
        std::array<Texture*, 9> textures = {};

        if (materialCmp.albedoTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[0] = &assetManager.getAsset<Texture>(materialCmp.albedoTextureID);
        }

        if (materialCmp.metallicRoughnessTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[1] = &assetManager.getAsset<Texture>(materialCmp.metallicRoughnessTextureID);
        }

        if (materialCmp.normalTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[3] = &assetManager.getAsset<Texture>(materialCmp.normalTextureID);
        }

        if (materialCmp.displacementTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[4] = &assetManager.getAsset<Texture>(materialCmp.displacementTextureID);
        }

        if (materialCmp.ambientOcclusionTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[5] = &assetManager.getAsset<Texture>(materialCmp.ambientOcclusionTextureID);
        }

        if (materialCmp.opacityTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[6] = &assetManager.getAsset<Texture>(materialCmp.opacityTextureID);
        }

        //TODO Fetch only once
        if (const Entity skyboxEntity(&scene, scene.view<SkyboxComponent>().front()); skyboxEntity) {
            if (const auto& skyboxCmp = skyboxEntity.getComponent<SkyboxComponent>(); skyboxCmp.irradianceCubemapID != AssetManager::INVALID_ASSET_ID) {
                textures[7] = &assetManager.getAsset<Texture>(skyboxCmp.irradianceCubemapID);
            }
        }

        if (const auto dirShadowComponent = scene.contextGet<DirectionalShadowComponent>()) {
            textures[8] = dirShadowComponent->properties.texture;
        }

        return textures;
    }
} // Kita
