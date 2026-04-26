#include "../../../../kitapch.h"
#include "SkyboxSystem.h"
#include "../../../../Core/Engine.h"
#include "../../../Util/CubemapUtil.h"
#include "../Components/SkyboxComponent.h"

namespace Kita {
    int SkyboxSystem::getOrder() {
        return Order::SKYBOX;
    }

    void SkyboxSystem::update(Scene& scene) {
    }

    void SkyboxSystem::render(Scene& scene) {
        const auto enttEntity = scene.view<SkyboxComponent>().front();
        if (enttEntity == entt::null) {
            return;
        }

        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        if (m_skyboxMeshID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxMeshID = assetManager.createAsset<Mesh>(Geometry::getCubeData());
        }
        if (m_skyboxBuilderShaderID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxBuilderShaderID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaSkyboxBuilderVertex.glsl"), Shader::frag("KitaSkyboxBuilderFragment.glsl")
            });
        }
        if (m_skyboxRenderShaderID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxRenderShaderID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaSkyboxRenderVertex.glsl"), Shader::frag("KitaSkyboxRenderFragment.glsl")
            });
        }

        auto entity = Entity(&scene, enttEntity);
        auto& skyboxComponent = entity.getComponent<SkyboxComponent>();

        if (m_skyboxToCubemapFBO == nullptr) {
            m_skyboxToCubemapFBO = FrameBuffer::createPtr();
            m_skyboxToCubemapFBO->createBuffer(CUBEMAP_FACE_RES, {{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}}}, true, 1);
            convertSkyboxTextureToCubemap(skyboxComponent); //TODO Release skybox texture
        }

        if (m_skyboxTextureID != skyboxComponent.skyboxID) {
            m_skyboxToCubemapFBO->resize(assetManager.getAsset<Texture>(skyboxComponent.skyboxID).getResolution());
            convertSkyboxTextureToCubemap(skyboxComponent);
        }

        renderer.getMainFramebuffer().bind();
        renderer.setDepthFunc(DepthFunction::LEQUAL);
        renderer.disableBufferWrite(BufferType::DEPTH);

        renderer.renderMesh(assetManager.getAsset<Mesh>(m_skyboxMeshID), assetManager.getAsset<Shader>(m_skyboxRenderShaderID), glm::mat4(1.0f),
                            {{&assetManager.getAsset<Texture>(skyboxComponent.cubemapID)}});

        renderer.enableBufferWrite(BufferType::DEPTH);
        renderer.setDepthFunc(DepthFunction::LESS);
        renderer.getMainFramebuffer().unbind();
    }

    void SkyboxSystem::convertSkyboxTextureToCubemap(SkyboxComponent& skyboxComponent) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        m_skyboxTextureID = skyboxComponent.skyboxID;

        renderer.setViewport(CUBEMAP_FACE_RES, false);
        m_skyboxToCubemapFBO->bind();

        if (skyboxComponent.cubemapID == AssetManager::INVALID_ASSET_ID) {
            skyboxComponent.cubemapID = assetManager.createAsset<Texture>(Texture::TextureType::CUBEMAP, CUBEMAP_FACE_RES);
        }

        const auto& cubemapTexture = assetManager.getAsset<Texture>(skyboxComponent.cubemapID);
        auto& skyboxTexture = assetManager.getAsset<Texture>(skyboxComponent.skyboxID);
        auto& builderShader = assetManager.getAsset<Shader>(m_skyboxBuilderShaderID);
        const auto& skyboxMesh = assetManager.getAsset<Mesh>(m_skyboxMeshID);

        builderShader.bind();
        builderShader.setUniformMat4("projection", CubemapUtil::getCubemapCaptureProjection());

        const std::vector<glm::mat4> captureViews = CubemapUtil::getCubemapCaptureViews();
        for (int i = 0; i < 6; ++i) {
            m_skyboxToCubemapFBO->attachCubemapFace(cubemapTexture.getTexture(), i);

            builderShader.setUniformMat4("view", captureViews[i]);

            renderer.renderMesh(skyboxMesh, builderShader, glm::mat4(1.0f), {{&skyboxTexture}});
        }
        m_skyboxToCubemapFBO->unbind();
        renderer.restoreViewport();
    }
} // Kita
