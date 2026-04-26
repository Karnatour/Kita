#include "../../../../kitapch.h"

#include "PostProcessingSystem.h"
#include "../../../../Core/Engine.h"

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

        renderer.getMainFramebuffer().unbind();
        renderer.disableCapability(Capability::DEPTH_TEST);
        renderer.clearBit({{ClearBit::COLOR}});

        renderer.renderMesh(assetManager.getAsset<Mesh>(m_quadMeshAssetID), assetManager.getAsset<Shader>(m_shaderAssetID), glm::mat4(1.0f), {});
        renderer.enableCapability(Capability::DEPTH_TEST);
    }
} // Kita
