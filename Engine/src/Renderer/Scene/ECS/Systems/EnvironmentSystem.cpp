#include "EnvironmentSystem.h"

#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../../../../Events/EventManager.h"
#include "../../../Util/CameraUtil.h"
#include "../Components/CameraComponent.h"
#include "../Components/EnvironmentComponent.h"

namespace Kita {
    int EnvironmentSystem::getOrder() {
        return Order::ENVIRONMENT;
    }

    void EnvironmentSystem::update([[maybe_unused]] Scene& scene) {
        if (Engine::getEngine()->isFirstFrame()) {
            EventManager::listenToEvent<ViewportResized>([this](const ViewportResized& event) { onViewportResized(event); });
        }
    }

    void EnvironmentSystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("Environment system render");
        const auto debugScope = DebugScope::createPtr();
        debugScope->scope("Environment pass");

        const auto activeCamera = Entity(&scene, scene.view<ActiveCamera>().front());
        if (!activeCamera) {
            return;
        }

        auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        if (m_quadMeshAssetID == AssetManager::INVALID_ASSET_ID) {
            m_quadMeshAssetID = assetManager.createAsset<Mesh>(Geometry::getQuadData());
        }

        if (m_shaderAssetID == AssetManager::INVALID_ASSET_ID) {
            m_shaderAssetID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaPostProcessVertex.glsl"), Shader::frag("KitaEnvironmentFragment.glsl")
            });
        }

        if (m_environmentUBO == nullptr) {
            m_environmentUBO = m_environmentUBO->createPtr();
            m_environmentUBO->createBuffer(sizeof(EnvironmentUBOLayout), nullptr);
        }

        if (m_environmentalFBO == nullptr) {
            m_environmentalFBO = m_environmentalFBO->createPtr();
            m_environmentalFBO->createEmptyBuffer();
            m_environmentalFBO->attachExternalTexture(BufferType::COLOR, *Engine::getEngine()->getRenderer().getMainFramebuffer().getColorTexture());
        }

        auto [fogColor, fogStart,fogEnd,fogDensity,fogMode] = scene.contextGet<EnvironmentComponent>()->properties.fogProperties;
        const auto environmentUBOLayout = EnvironmentUBOLayout{.fogColor = glm::vec4(fogColor, 1.0f), .fogParams = glm::vec4(fogStart, fogEnd, fogDensity, static_cast<float>(fogMode))};

        m_environmentUBO->bind(3);
        m_environmentUBO->upload(sizeof(EnvironmentUBOLayout), &environmentUBOLayout);

        m_environmentalFBO->bind();

        auto& shader = assetManager.getAsset<Shader>(m_shaderAssetID);
        shader.bind();
        auto cameraProperties = activeCamera.getComponent<CameraComponent>().properties;

        auto viewRotation = glm::mat4(glm::mat3(CameraUtil::getViewMatrix(cameraProperties)));
        auto projection = CameraUtil::getProjectionMatrix(cameraProperties, renderer.getViewport());
        shader.setUniformMat4("invViewProj", glm::inverse(projection * viewRotation));

        renderer.disableCapability(Capability::DEPTH_TEST);
        renderer.disableBufferWrite(BufferType::DEPTH);
        renderer.enableCapability(Capability::BLEND);
        renderer.setBlendFunc(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_SRC_ALPHA);

        renderer.renderMesh(assetManager.getAsset<Mesh>(m_quadMeshAssetID), shader, glm::mat4(1.0f), {{renderer.getMainFramebuffer().getDepthTexture()}});

        renderer.disableCapability(Capability::BLEND);
        renderer.enableBufferWrite(BufferType::DEPTH);
        renderer.enableCapability(Capability::DEPTH_TEST);

        m_environmentalFBO->unbind();
    }

    void EnvironmentSystem::onViewportResized(const ViewportResized&) {
        if (m_environmentalFBO != nullptr) {
            m_environmentalFBO->attachExternalTexture(BufferType::COLOR, *Engine::getEngine()->getRenderer().getMainFramebuffer().getColorTexture());
        }
    }
} // Kita
