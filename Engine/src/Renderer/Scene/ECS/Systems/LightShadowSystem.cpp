#include "LightShadowSystem.h"

#include "TerrainSystem.h"
#include "../../Scene.h"
#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../../../Util/LightUtil.h"
#include "../Components/Components.h"

namespace Kita {
    int LightShadowSystem::getOrder() {
        return Order::LIGHTSHADOW;
    }

    void LightShadowSystem::update([[maybe_unused]] Scene& scene) {
    }

    void LightShadowSystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("LightShadow system render");
        const auto debugScope = DebugScope::createPtr();
        debugScope->scope("LightShadow pass");

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
            m_lightSSBO = ShaderStorageBuffer::createPtr(sizeof(glm::ivec4) + sizeof(LightSSBOLayout) * LightSSBOLayout::INITIAL_LIGHT_CAPACITY_COUNT, nullptr);
        }

        if (m_directionalShadowUBO == nullptr) {
            m_directionalShadowUBO = UniformBuffer::createPtr(sizeof(DirectionalShadowUBOLayout), nullptr);
        }

        auto dirShadowComponent = scene.contextGet<DirectionalShadowComponent>();
        if (!dirShadowComponent) {
            return;
        }
        const DirectionalShadowProperties& properties = dirShadowComponent->properties;
        const int cascadeCount = properties.cascadeCount;

        if (m_lightFBO == nullptr || m_cascadeCount != cascadeCount) {
            m_cascadeCount = cascadeCount;

            m_lightFBO = FrameBuffer::createPtr(properties.resolution, {{.bufferType = BufferType::DEPTH, .attachmentType = FrameBuffer::AttachType::TEXTURE}}, true, m_cascadeCount);

            const std::string cascadeCountStr = std::to_string(DirectionalShadowUBOLayout::MAX_CASCADES);
            auto& assetManager = Engine::getEngine()->getAssetManager();
            m_CSMShaderAssetID = assetManager.getOrCreateAsset<Shader>(AssetKey{"KitaCSMVertex.glsl", "KitaEmptyFragment.glsl"}, {}, std::vector{Shader::vert({Shader::define("MAX_CASCADES", cascadeCountStr)}), Shader::frag()}).id;
        }
    }

    void LightShadowSystem::renderShadowPass(Scene& scene) {
        KITA_ENGINE_PROFILE("Render shadow pass");

        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        auto& shader = assetManager.getAsset<Shader>(m_CSMShaderAssetID);

        const auto dirShadowComponent = scene.contextGet<DirectionalShadowComponent>();
        if (!dirShadowComponent) {
            return;
        }
        DirectionalShadowProperties& properties = dirShadowComponent->properties;

        m_lightFBO->bind();
        renderer.enableCapability(Capability::DEPTH_CLAMP);
        renderer.setViewport(properties.resolution, false);
        renderer.clearBit({{ClearBit::DEPTH}});

        shader.bind();
        for (int i = 0; i < m_cascadeCount; ++i) {
            shader.setUniformInt("cascadeIndex", i);
            if (const TerrainSystem* terrainSystem = scene.getSystem<TerrainSystem>(); terrainSystem != nullptr) {
                shader.setUniformMat4("model", glm::mat4(1.0f));
                terrainSystem->getChunkRenderPool().render(glm::vec2(terrainSystem->getCameraChunkPos().getChunkPos().first, terrainSystem->getCameraChunkPos().getChunkPos().second), terrainSystem->getFrustum(), 400.0f); //TODO Move to variable
            }

            // Set cull mode to front to reduce shadow artifacts (doesn't affect terrain so keep BACK for it)
            renderer.setCullMode(CullMode::FRONT);
            for (const auto [entity,children, transformation] : scene.view<ChildrenComponent, TransformationComponent, RenderInShadowPass>().each()) {
                for (const auto child : children.children) {
                    if (Entity childEntity(&scene, child); childEntity.hasAllComponents<MeshComponent, MaterialComponent>()) {
                        Mesh& mesh = assetManager.getAsset<Mesh>(childEntity.getComponent<MeshComponent>().meshID);

                        renderer.renderMesh(mesh, shader, transformation.worldModel, {}, true);
                    }
                }
            }
        }

        renderer.setCullMode(CullMode::BACK);
        renderer.disableCapability(Capability::DEPTH_CLAMP);

        renderer.restoreViewport();
        m_lightFBO->unbind();

        properties.texture = m_lightFBO->getDepthTexture();
    }

    void LightShadowSystem::uploadLightData(Scene& scene) {
        KITA_ENGINE_PROFILE("Upload light data");

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
        KITA_ENGINE_PROFILE("Upload directional light data");

        const auto dirShadowComponent = scene.contextGet<DirectionalShadowComponent>();
        if (!dirShadowComponent) {
            return;
        }

        const Entity cameraEntity(&scene, scene.view<CameraComponent, ActiveCamera>().front());

        auto& [cascadeSplitDistances, lightSpaceMatrices, shadowMapResolution,texture, cascadeCount, shadowDistance, lambda] = dirShadowComponent->properties;
        DirectionalShadowUBOLayout dirLightUBOData{};

        const CameraProperties& cameraProperties = cameraEntity.getComponent<CameraComponent>().properties;

        const Entity dirShadowEntity(&scene, scene.view<LightComponent, DirectionalLight>().front());
        const glm::vec3 lightDir = dirShadowEntity.getComponent<LightComponent>().properties.direction;

        //extract
        for (int i = 0; i < m_cascadeCount; i++) {
            const float p = static_cast<float>(i + 1) / static_cast<float>(m_cascadeCount);
            const float logSplit = cameraProperties.zNear * std::pow(shadowDistance / cameraProperties.zNear, p);
            const float linSplit = cameraProperties.zNear + (shadowDistance - cameraProperties.zNear) * p;
            cascadeSplitDistances[i] = glm::mix(linSplit, logSplit, lambda);
        }

        //extract
        for (int i = 0; i < m_cascadeCount; i++) {
            const float nearPlane = (i == 0) ? cameraProperties.zNear : cascadeSplitDistances[i - 1];
            const float farPlane = cascadeSplitDistances[i];

            auto [matrix, radius, depthPerUnit] = LightUtil::getLightSpaceMatrix(cameraProperties, nearPlane, farPlane, lightDir, Engine::getEngine()->getRenderer().getViewport(), shadowMapResolution);;
            lightSpaceMatrices[i] = matrix;

            dirLightUBOData.cascadeSplitDistances[i].x = cascadeSplitDistances[i];
            dirLightUBOData.cascadeSplitDistances[i].y = radius;
            dirLightUBOData.cascadeSplitDistances[i].z = depthPerUnit;

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
            .diffuse = glm::vec4(properties.diffuse.x, properties.diffuse.y, properties.diffuse.z, 0.0f),
            .cutOff = glm::vec4(properties.cutOff.x, properties.cutOff.y, 0.0f, 0.0f),
            .params = glm::ivec4(properties.lightType, castsShadows ? 1 : 0, castsShadows ? m_castsShadowsCount++ : -1, -1)
        };
    }
} // Kita
