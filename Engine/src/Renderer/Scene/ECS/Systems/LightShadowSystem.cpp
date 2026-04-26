#include "../../../../kitapch.h"
#include "LightShadowSystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/LightShadowComponents.h"
#include "../Components/MeshComponent.h"
#include "../Components/RenderTags.h"
#include "../Components/TransformationComponent.h"

namespace Kita {
    int LightShadowSystem::getOrder() {
        return Order::LIGHTSHADOW;
    }

    void LightShadowSystem::update(Scene& scene) {
    }

    void LightShadowSystem::render(Scene& scene) {
        m_castsShadowsCount = 0;
        createBuffersIfMissing(scene);
        if (m_lightFBO != nullptr) {
            renderShadowPass(scene);

            uploadLightData(scene);
            uploadDirectionalLightData(scene);
        }
    }

    void LightShadowSystem::createBuffersIfMissing(Scene& scene) {
        if (m_lightSSBO == nullptr) {
            m_lightSSBO = ShaderStorageBuffer::createPtr();
            m_lightSSBO->createBuffer(sizeof(LightSSBOLayout) * LightSSBOLayout::INITIAL_LIGHT_CAPACITY_COUNT, nullptr);
        }

        if (m_directionalShadowUBO == nullptr) {
            m_directionalShadowUBO = UniformBuffer::createPtr();
            m_directionalShadowUBO->createBuffer(sizeof(DirectionalShadowUBOLayout), nullptr);
        }

        if (const auto enttEntity = scene.view<DirectionalShadowComponent>().front(); m_lightFBO == nullptr && enttEntity != entt::null) {
            m_lightFBO = FrameBuffer::createPtr();
            m_lightFBO->createBuffer(m_lightFBOresolution, {{{BufferType::DEPTH, FrameBuffer::AttachType::TEXTURE}}}, true, CASCADE_COUNT);
        }
    }

    void LightShadowSystem::renderShadowPass(Scene& scene) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        const std::initializer_list shaderInfos = {
            Shader::vert("KitaCSMVertex.glsl"),
            Shader::geom("KitaCSMGeometry.glsl", {Shader::define("NUM_CASCADES", std::to_string(CASCADE_COUNT))}),
            Shader::frag("KitaEmptyFragment.glsl")
        };
        m_CSMShaderAssetID = assetManager.createAsset<Shader>(shaderInfos);

        auto& shader = assetManager.getAsset<Shader>(m_CSMShaderAssetID);

        m_lightFBO->bind();
        renderer.setViewport(DirectionalShadowProperties::RESOLUTION, false);
        renderer.clearBit({{ClearBit::DEPTH}});
        for (const auto [entity,meshComponent, transformationComponent] : scene.view<MeshComponent, TransformationComponent, RenderInShadowPass>().each()) {
            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComponent.meshID), shader, transformationComponent.model);
        }

        renderer.restoreViewport();
        m_lightFBO->unbind();
    }

    void LightShadowSystem::uploadLightData(Scene& scene) {
        const auto lightsView = scene.view<LightComponent>();
        const size_t count = lightsView.size<>();
        std::vector<LightSSBOLayout> lightSSBOData;
        lightSSBOData.reserve(count);

        for (const auto& [entity,lightComponent] : lightsView.each()) {
            lightSSBOData.emplace_back(convertLightToSSBOLayout(Entity(&scene, entity), lightComponent.properties));
        }

        m_lightSSBO->bind(0);
        m_lightSSBO->upload(lightSSBOData.size(), lightSSBOData.data());
    }

    void LightShadowSystem::uploadDirectionalLightData(Scene& scene) const {
        Entity dirLightEntity(&scene, scene.view<LightComponent, DirectionalShadowComponent>().front());
        const auto [cascadeSplitDistances, lightSpaceMatrices] = dirLightEntity.getComponent<DirectionalShadowComponent>().properties;
        DirectionalShadowUBOLayout dirLightUBOData{};

        for (int i = 0; i < CASCADE_COUNT; i++) {
            dirLightUBOData.cascadeSplitDistances[i].x = cascadeSplitDistances[i];
            dirLightUBOData.lightSpaceMatrices[i] = lightSpaceMatrices[i];
        }
        dirLightUBOData.params = glm::ivec4(CASCADE_COUNT, -1, -1, -1);

        m_directionalShadowUBO->bind(1);
        m_directionalShadowUBO->upload(sizeof(DirectionalShadowUBOLayout), &dirLightUBOData);
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
