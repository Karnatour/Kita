#include "../../../../kitapch.h"
#include "GeometrySystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/Components.h"

namespace Kita {
    struct DirectionalShadowProperties;

    int GeometrySystem::getOrder() {
        return Order::GEOMETRY;
    }

    void GeometrySystem::update(Scene& scene) {
    }

    void GeometrySystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("Geometry system render");

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
                    shader.setUniformFloat("iblIntensity", Entity(&scene, scene.view<SceneComponent>().front()).getComponent<SceneComponent>().properties.iblIntensity); //TODO Move to UBO ?

                    renderer.renderMesh(mesh, shader, transformation.worldModel, fetchTextures(assetManager, material, scene));
                }
            }
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

        if (const Entity skyboxEntity(&scene, scene.view<SkyboxComponent>().front()); skyboxEntity) {
            if (const auto& skyboxCmp = skyboxEntity.getComponent<SkyboxComponent>(); skyboxCmp.irradianceCubemapID != AssetManager::INVALID_ASSET_ID) {
                textures[3] = &assetManager.getAsset<Texture>(skyboxCmp.irradianceCubemapID);
            }
        }

        if (const Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front()); dirShadowEntity) {
            textures[4] = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties.texture;
        }

        return textures;
    }
} // Kita
