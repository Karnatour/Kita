#include "PostProcessingSystem.h"

#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../Components/PostProcessingComponent.h"

namespace Kita {
    int PostProcessingSystem::getOrder() {
        return Order::POST;
    }

    void PostProcessingSystem::update([[maybe_unused]] Scene& scene) {
    }

    void PostProcessingSystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("Post processing system render");
        const auto debugScope = DebugScope::createPtr();
        debugScope->scope("Post processing pass");

        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        if (m_quadMeshAssetID == AssetManager::INVALID_ASSET_ID) {
            m_quadMeshAssetID = assetManager.getOrCreateAsset<Mesh>(AssetKey{Geometry::getQuadKey()}, {}, Geometry::getQuadData()).id;
        }
        if (m_shaderAssetID == AssetManager::INVALID_ASSET_ID) {
            m_shaderAssetID = assetManager.getOrCreateAsset<Shader>(AssetKey{"KitaPostProcessVertex.glsl", "KitaPostProcessFragment.glsl"}, {}, std::vector{Shader::vert(), Shader::frag()}).id;
        }

        Engine::getEngine()->isEditor() ? renderer.getOutputFramebuffer().bind() : renderer.getMainFramebuffer().unbind();

        renderer.disableCapability(Capability::DEPTH_TEST);
        renderer.clearBit({{ClearBit::COLOR}});
        auto& shader = assetManager.getAsset<Shader>(m_shaderAssetID);
        shader.bind();
        shader.setUniformFloat("exposure", scene.contextGet<PostProcessingComponent>()->properties.exposure); //TOOD Move to UBO ?
        renderer.renderMesh(assetManager.getAsset<Mesh>(m_quadMeshAssetID), shader, glm::mat4(1.0f), {{renderer.getMainFramebuffer().getColorTexture()}});
        renderer.enableCapability(Capability::DEPTH_TEST);

        if (Engine::getEngine()->isEditor()) {
            renderer.getOutputFramebuffer().unbind();
            renderer.clearBit({{ClearBit::COLOR}});
        }
    }
} // Kita
