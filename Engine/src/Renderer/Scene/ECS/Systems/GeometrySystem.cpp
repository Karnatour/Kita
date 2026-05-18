#include "../../../../kitapch.h"
#include "GeometrySystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/MaterialComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/RenderTags.h"
#include "../Components/TransformationComponent.h"
#include "../Components/LightShadowComponents.h"

namespace Kita {
    struct DirectionalShadowProperties;

    int GeometrySystem::getOrder() {
        return Order::GEOMETRY;
    }

    void GeometrySystem::update(Scene& scene) {
    }

    void GeometrySystem::render(Scene& scene) {
        // lazy creation on render thread
        if (m_phongPropertiesUBO == nullptr) {
            m_phongPropertiesUBO = UniformBuffer::createPtr();
            m_phongPropertiesUBO->createBuffer(sizeof(PhongPropertiesUBOLayout), nullptr);
        }

        const auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        renderer.getMainFramebuffer().bind();
    
        for (const auto& [entityID, meshComp,materialCmp,transformationComp] : scene.view<MeshComponent, MaterialComponent, TransformationComponent, RenderInMainPass>().each()) {
            auto m_phongPropertiesData = PhongPropertiesUBOLayout{
                .ambient = glm::vec4(materialCmp.properties.ambient, 0.0f),
                .diffuse = glm::vec4(materialCmp.properties.diffuse, 0.0f),
                .specular = glm::vec4(materialCmp.properties.specular, 0.0f),
                .params = glm::vec4(materialCmp.properties.shininess, 0.0f, 0.0f, 0.0f)
            };
            m_phongPropertiesUBO->bind(2);
            m_phongPropertiesUBO->upload(sizeof(PhongPropertiesUBOLayout), &m_phongPropertiesData);
            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComp.meshID), assetManager.getAsset<Shader>(materialCmp.shaderID), transformationComp.model,
                                fetchTextures(assetManager, materialCmp, scene));
        }

        renderer.getMainFramebuffer().unbind();
    }

    std::array<Texture*, 4> GeometrySystem::fetchTextures(const AssetManager& assetManager, const MaterialComponent& materialCmp, Scene& scene) {
        std::array<Texture*, 4> textures = {};

        if (materialCmp.diffuseTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[0] = &assetManager.getAsset<Texture>(materialCmp.diffuseTextureID);
        }

        if (materialCmp.specularTextureID != AssetManager::INVALID_ASSET_ID) {
            textures[1] = &assetManager.getAsset<Texture>(materialCmp.specularTextureID);
        }

        if (materialCmp.normalTtextureID != AssetManager::INVALID_ASSET_ID) {
            textures[2] = &assetManager.getAsset<Texture>(materialCmp.normalTtextureID);
        }

        if (Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front()); dirShadowEntity.getEnttEntityID() != entt::null) {
            textures[3] = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties.texture;
        }

        return textures;
    }
} // Kita
