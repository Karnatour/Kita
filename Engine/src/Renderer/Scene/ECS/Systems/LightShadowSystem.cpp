#include "LightShadowSystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/LightShadowComponents.h"
#include "../Components/MeshComponent.h"
#include "../Components/RenderTags.h"
#include "../Components/TransformationComponent.h"

namespace Kita {
    void LightShadowSystem::update(Scene& scene) {
    }

    void LightShadowSystem::render(Scene& scene) {
        m_castsShadowsCount = 0;
        if (m_lightSSBO == nullptr) {
            m_lightSSBO = ShaderStorageBuffer::createPtr();
            m_lightSSBO->createBuffer(sizeof(LightSSBOLayout) * LightSSBOLayout::INITIAL_LIGHT_CAPACITY_COUNT, nullptr);
        }

        if (m_directionalShadowUBO == nullptr) {
            m_directionalShadowUBO = UniformBuffer::createPtr();
            m_directionalShadowUBO->createBuffer(sizeof(DirectionalShadowUBOLayout), nullptr);
        }

        renderShadowPass(scene);

        uploadLightData(scene);
        uploadDirectionalLightData(scene);
    }

    void LightShadowSystem::renderShadowPass(Scene& scene) {
        if (m_lightFBO == nullptr) {
            m_lightFBO = FrameBuffer::createPtr();
            const int cascadeCount = Entity(&scene, scene.view<DirectionalShadowComponent>().front()).getComponent<DirectionalShadowComponent>().properties.cascadeCount;
            m_lightFBO->createBuffer(m_lightFBOresolution, std::array{FrameBuffer::AttachInfo{BufferType::DEPTH, FrameBuffer::AttachType::TEXTURE}}, true, cascadeCount);
        }

        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();
        const int cascadeCount = Entity(&scene, scene.view<DirectionalShadowComponent>().front()).getComponent<DirectionalShadowComponent>().properties.cascadeCount;

        const std::array shaderInfos = {
            Shader::vert("KitaCSMVertex.glsl"),
            Shader::geom("KitaCSMGeometry.glsl", {Shader::define("NUM_CASCADES", std::to_string(cascadeCount))}),
            Shader::frag("KitaEmptyFragment.glsl")
        };
        auto& shader = assetManager.getOrCreateAsset<Shader>(std::nullopt, false, shaderInfos);

        m_lightFBO->bind();
        renderer.getRendererAPI().clearBit(std::array{ClearBit::DEPTH});
        for (const auto [entity,meshComponent, transformationComponent] : scene.view<MeshComponent, TransformationComponent, RenderInShadowPass>().each()) {
            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComponent.mesh), shader, transformationComponent.model,);
        }

        m_lightFBO->unbind();
    }

    void LightShadowSystem::uploadLightData(Scene& scene) {
        const auto lightsView = scene.view<LightComponent>();
        const size_t count = lightsView.size<>();
        std::vector<LightSSBOLayout> lightSSBOData;
        lightSSBOData.reserve(count);

        for (auto [entity,lightComponent] : lightsView.each()) {
            lightSSBOData.emplace_back(convertLightToSSBOLayout(Entity(&scene, entity), lightComponent.properties));
        }

        m_lightSSBO->bind(0);
        m_lightSSBO->upload(sizeof(lightSSBOData), lightSSBOData.data());
    }

    void LightShadowSystem::uploadDirectionalLightData(Scene& scene) const {
        Entity dirLightEntity(&scene, scene.view<LightComponent, DirectionalShadowComponent>().front());
        const auto [cascadeCount, cascadeSplitDistances, lightSpaceMatrices] = dirLightEntity.getComponent<DirectionalShadowComponent>().properties;
        DirectionalShadowUBOLayout dirLightUBOData{};

        for (int i = 0; i < cascadeCount; i++) {
            dirLightUBOData.cascadeSplitDistances[i].x = cascadeSplitDistances[i];
            dirLightUBOData.lightSpaceMatrices[i] = lightSpaceMatrices[i];
        }
        dirLightUBOData.params = glm::ivec4(cascadeCount, -1, -1, -1);

        m_directionalShadowUBO->bind(1);
        m_directionalShadowUBO->createBuffer(sizeof(DirectionalShadowUBOLayout), &dirLightUBOData);
    }

    LightShadowSystem::LightSSBOLayout LightShadowSystem::convertLightToSSBOLayout(const Entity entity, const LightProperties& properties) {
        const bool castsShadows = entity.hasAllComponents<CastsShadows>();
        return {
            .position = glm::vec4(properties.position.x, properties.position.y, properties.position.z, 0.0f),
            .direction = glm::vec4(properties.direction.x, properties.direction.y, properties.direction.z, 0.0f),
            .attenuation = glm::vec4(properties.attenuation.x, properties.attenuation.y, properties.attenuation.z, 0.0f),
            .ambient = glm::vec4(properties.ambient.x, properties.ambient.y, properties.ambient.z, 0.0f),
            .diffuse = glm::vec4(properties.diffuse.x, properties.diffuse.y, properties.diffuse.z, 0.0f),
            .specular = glm::vec4(properties.specular.x, properties.specular.y, properties.specular.z, 0.0f),
            .cutOff = glm::vec4(properties.cutOff.x, properties.cutOff.y, 0.0f, 0.0f),
            .params = glm::ivec4(properties.lightType, castsShadows ? 1 : 0, castsShadows ? m_castsShadowsCount++ : -1, -1)
        };
    }
} // Kita
