#include "../../../../kitapch.h"
#include "LightShadowSystem.h"

#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../../../Util/LightUtil.h"
#include "../Components/LightShadowComponents.h"
#include "../Components/CameraComponent.h"
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
            uploadLightData(scene);
            uploadDirectionalLightData(scene);
            renderShadowPass(scene);
        }
    }

    void LightShadowSystem::createBuffersIfMissing(Scene& scene) {
        if (m_lightSSBO == nullptr) {
            m_lightSSBO = ShaderStorageBuffer::createPtr();
            m_lightSSBO->createBuffer(sizeof(glm::ivec4) + sizeof(LightSSBOLayout) * LightSSBOLayout::INITIAL_LIGHT_CAPACITY_COUNT, nullptr);
        }

        if (m_directionalShadowUBO == nullptr) {
            m_directionalShadowUBO = UniformBuffer::createPtr();
            m_directionalShadowUBO->createBuffer(sizeof(DirectionalShadowUBOLayout), nullptr);
        }

        Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front());
        const DirectionalShadowProperties& properties = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties;
        int cascadeCount = properties.cascadeCount;

        if (m_lightFBO == nullptr || m_cascadeCount != cascadeCount) {
            m_cascadeCount = cascadeCount;

            m_lightFBO = FrameBuffer::createPtr();
            m_lightFBO->createBuffer(properties.resolution, {{{BufferType::DEPTH, FrameBuffer::AttachType::TEXTURE}}}, true, m_cascadeCount);
            const std::initializer_list shaderInfos = {
                Shader::vert("KitaCSMVertex.glsl"),
                Shader::geom("KitaCSMGeometry.glsl", {
                                 Shader::define("NUM_CASCADES", std::to_string(m_cascadeCount)),
                                 Shader::define("MAX_CASCADES", std::to_string(DirectionalShadowUBOLayout::MAX_CASCADES))
                             }),
                Shader::frag("KitaEmptyFragment.glsl")
            };
            m_CSMShaderAssetID = Engine::getEngine()->getAssetManager().createAsset<Shader>(shaderInfos);
        }
    }

    void LightShadowSystem::renderShadowPass(Scene& scene) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        auto& shader = assetManager.getAsset<Shader>(m_CSMShaderAssetID);

        Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front());
        DirectionalShadowProperties& properties = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties;

        m_lightFBO->bind();
        renderer.setViewport(properties.resolution, false);
        renderer.clearBit({{ClearBit::DEPTH}});
        renderer.setCullMode(CullMode::FRONT);
        for (const auto [entity,meshComponent, transformationComponent] : scene.view<MeshComponent, TransformationComponent, RenderInShadowPass>().each()) {
            renderer.renderMesh(assetManager.getAsset<Mesh>(meshComponent.meshID), shader, transformationComponent.model);
        }
        renderer.setCullMode(CullMode::BACK);

        renderer.restoreViewport();
        m_lightFBO->unbind();

        properties.texture = m_lightFBO->getDepthTexture();
    }

    void LightShadowSystem::uploadLightData(Scene& scene) {
        const auto lightsView = scene.view<LightComponent>();
        const size_t count = lightsView.size<>();

        std::vector<LightSSBOLayout> lightSSBOData;
        lightSSBOData.reserve(count);

        for (const auto& [entity,lightComponent] : lightsView.each()) {
            lightSSBOData.emplace_back(convertLightToSSBOLayout(Entity(&scene, entity), lightComponent.properties));
        }

        std::vector<std::byte> buffer;
        buffer.resize(sizeof(glm::ivec4) + lightSSBOData.size() * sizeof(LightSSBOLayout));
        const auto header = glm::ivec4(lightSSBOData.size(), 0, 0, 0);
        std::memcpy(buffer.data(), &header, sizeof(glm::ivec4));
        std::memcpy(buffer.data() + sizeof(glm::ivec4), lightSSBOData.data(), lightSSBOData.size() * sizeof(LightSSBOLayout));

        m_lightSSBO->bind(0);
        m_lightSSBO->upload(buffer.size(), buffer.data());
    }

    void LightShadowSystem::uploadDirectionalLightData(Scene& scene) const {
        Entity dirLightEntity(&scene, scene.view<LightComponent, DirectionalShadowComponent>().front());
        Entity cameraEntity(&scene, scene.view<CameraComponent, ActiveCamera>().front());

        auto& [cascadeSplitDistances, lightSpaceMatrices, shadowMapResolution,texture, cascadeCount] = dirLightEntity.getComponent<DirectionalShadowComponent>().properties;
        DirectionalShadowUBOLayout dirLightUBOData{};

        const CameraProperties& cameraProperties = cameraEntity.getComponent<CameraComponent>().properties;
        glm::vec3 lightDir = dirLightEntity.getComponent<LightComponent>().properties.direction;

        //extract
        for (int i = 0; i < m_cascadeCount; i++) {
            const float p = static_cast<float>((i + 1)) / static_cast<float>(m_cascadeCount);

            const float logSplit = cameraProperties.zNear * std::pow(cameraProperties.zFar / cameraProperties.zNear, p);
            const float linSplit = cameraProperties.zNear + (cameraProperties.zFar - cameraProperties.zNear) * p;

            float t = 0.5f;
            cascadeSplitDistances[i] = glm::mix(linSplit, logSplit, t);
        }

        //extract
        for (int i = 0; i < m_cascadeCount; i++) {
            const float nearPlane = (i == 0) ? cameraProperties.zNear : cascadeSplitDistances[i - 1];
            const float farPlane = cascadeSplitDistances[i];

            lightSpaceMatrices[i] = LightUtil::getLightSpaceMatrix(cameraProperties, nearPlane, farPlane, lightDir, Engine::getEngine()->getRenderer().getViewport(), shadowMapResolution);

            dirLightUBOData.cascadeSplitDistances[i].x = cascadeSplitDistances[i];
            dirLightUBOData.lightSpaceMatrices[i] = lightSpaceMatrices[i];
        }

        dirLightUBOData.params = glm::ivec4(m_cascadeCount, -1, -1, -1);

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
