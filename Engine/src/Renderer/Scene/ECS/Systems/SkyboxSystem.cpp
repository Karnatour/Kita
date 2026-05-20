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
        auto entity = Entity(&scene,scene.view<SkyboxComponent>().front());
        if (!entity) {
            return;
        }

        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        createAssetsIfNotExists();

        auto& skyboxComponent = entity.getComponent<SkyboxComponent>();

        if (m_cubemapCaptureFBO == nullptr) {
            m_cubemapCaptureFBO = FrameBuffer::createPtr();
            //temp attachments so FBO is complete
            m_cubemapCaptureFBO->createBuffer(
                {1,1},
                {{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}, {BufferType::DEPTH, FrameBuffer::AttachType::RENDERBUFFER}}},
                true, 1);
            convertSkyboxTextureToCubemap(skyboxComponent); //TODO Release skybox texture
            convertCubemapToIrradance(skyboxComponent);
        }

        if (m_skyboxTextureID != skyboxComponent.skyboxID) {
            convertSkyboxTextureToCubemap(skyboxComponent);
            convertCubemapToIrradance(skyboxComponent);
        }

        renderer.getMainFramebuffer().bind();
        renderer.setDepthFunc(DepthFunction::LEQUAL);
        renderer.disableBufferWrite(BufferType::DEPTH);

        renderer.renderMesh(assetManager.getAsset<Mesh>(m_skyboxMeshID), assetManager.getAsset<Shader>(m_skyboxRenderShaderID), glm::mat4(1.0f),
                            {{&assetManager.getAsset<Texture>(skyboxComponent.irradianceCubemapID)}});

        renderer.enableBufferWrite(BufferType::DEPTH);
        renderer.setDepthFunc(DepthFunction::LESS);
        renderer.getMainFramebuffer().unbind();
    }

    void SkyboxSystem::convertSkyboxTextureToCubemap(SkyboxComponent& skyboxComponent) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        m_skyboxTextureID = skyboxComponent.skyboxID;

        m_cubemapCaptureFBO->resize(CUBEMAP_FACE_RES);
        renderer.setViewport(CUBEMAP_FACE_RES, false);
        m_cubemapCaptureFBO->bind();

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
            m_cubemapCaptureFBO->attachCubemapFace(cubemapTexture.getTexture(), i);

            builderShader.setUniformMat4("view", captureViews[i]);

            renderer.clearBit({{ClearBit::COLOR, ClearBit::DEPTH}});
            renderer.renderMesh(skyboxMesh, builderShader, glm::mat4(1.0f), {{&skyboxTexture}});
        }
        m_cubemapCaptureFBO->unbind();
        renderer.restoreViewport();
    }

    void SkyboxSystem::convertCubemapToIrradance(SkyboxComponent& skyboxComponent) {
        auto& renderer = Engine::getEngine()->getRenderer();
        auto& assetManager = Engine::getEngine()->getAssetManager();

        m_cubemapCaptureFBO->resize(IRRADIANCE_RES);
        renderer.setViewport(IRRADIANCE_RES, false);
        m_cubemapCaptureFBO->bind();

        if (skyboxComponent.irradianceCubemapID == AssetManager::INVALID_ASSET_ID) {
            skyboxComponent.irradianceCubemapID = assetManager.createAsset<Texture>(Texture::TextureType::CUBEMAP, IRRADIANCE_RES);
        }

        auto& inputCubemapTexture = assetManager.getAsset<Texture>(skyboxComponent.cubemapID);
        const auto& irradianceCubemapTexture = assetManager.getAsset<Texture>(skyboxComponent.irradianceCubemapID);
        auto& convertShader = assetManager.getAsset<Shader>(m_skyboxBuilderIrradianceShaderID);
        const auto& skyboxMesh = assetManager.getAsset<Mesh>(m_skyboxMeshID);

        convertShader.bind();
        convertShader.setUniformMat4("projection", CubemapUtil::getCubemapCaptureProjection());

        const std::vector<glm::mat4> captureViews = CubemapUtil::getCubemapCaptureViews();
        for (int i = 0; i < 6; ++i) {
            m_cubemapCaptureFBO->attachCubemapFace(irradianceCubemapTexture.getTexture(), i);

            convertShader.setUniformMat4("view", captureViews[i]);

            renderer.clearBit({{ClearBit::COLOR, ClearBit::DEPTH}});
            renderer.renderMesh(skyboxMesh, convertShader, glm::mat4(1.0f), {{&inputCubemapTexture}});
        }

        m_cubemapCaptureFBO->unbind();
        renderer.restoreViewport();
    }

    void SkyboxSystem::createAssetsIfNotExists() {
        auto& assetManager = Engine::getEngine()->getAssetManager();

        if (m_skyboxMeshID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxMeshID = assetManager.createAsset<Mesh>(Geometry::getCubeData());
        }
        if (m_skyboxBuilderShaderID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxBuilderShaderID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaSkyboxBuilderVertex.glsl"), Shader::frag("KitaSkyboxBuilderFragment.glsl")
            });
        }
        if (m_skyboxBuilderIrradianceShaderID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxBuilderIrradianceShaderID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaSkyboxBuilderVertex.glsl"), Shader::frag("KitaSkyboxBuilderIrradianceFragment.glsl")
            });
        }
        if (m_skyboxRenderShaderID == AssetManager::INVALID_ASSET_ID) {
            m_skyboxRenderShaderID = assetManager.createAsset<Shader>(std::initializer_list{
                Shader::vert("KitaSkyboxRenderVertex.glsl"), Shader::frag("KitaSkyboxRenderFragment.glsl")
            });
        }
    }
} // Kita
