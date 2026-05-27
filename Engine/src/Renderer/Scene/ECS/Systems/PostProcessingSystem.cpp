#include "../../../../kitapch.h"

#include "PostProcessingSystem.h"
#include "../../../../Core/Engine.h"
#include "../Components/PostProcessingComponent.h"

namespace Kita {
    int PostProcessingSystem::getOrder() {
        return Order::POST;
    }

    void PostProcessingSystem::update(Scene& scene) {
    }

    void PostProcessingSystem::render(Scene& scene) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        if (m_quadMeshAssetID == AssetManager::INVALID_ASSET_ID) {
            m_quadMeshAssetID = assetManager.createAsset<Mesh>(Geometry::getQuadData());
        }
        if (m_shaderAssetID == AssetManager::INVALID_ASSET_ID) {
            m_shaderAssetID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaPostProcessVertex.glsl"), Shader::frag("KitaPostProcessFragment.glsl")
            });
        }

        Engine::getEngine()->isEditor() ? renderer.getOutputFramebuffer().bind() : renderer.getMainFramebuffer().unbind();

        renderer.disableCapability(Capability::DEPTH_TEST);
        renderer.clearBit({{ClearBit::COLOR}});
        auto& shader = assetManager.getAsset<Shader>(m_shaderAssetID);
        shader.bind();
        shader.setUniformFloat("exposure", Entity(&scene, scene.view<PostProcessingComponent>().front()).getComponent<PostProcessingComponent>().properties.exposure); //TOOD Move to UBO ?
        renderer.renderMesh(assetManager.getAsset<Mesh>(m_quadMeshAssetID), shader, glm::mat4(1.0f), {{renderer.getMainFramebuffer().getColorTexture()}});
        renderer.enableCapability(Capability::DEPTH_TEST);

        if (Engine::getEngine()->isEditor()) {
            renderer.getOutputFramebuffer().unbind();
            renderer.clearBit({{ClearBit::COLOR}});
        }
    }
} // Kita
