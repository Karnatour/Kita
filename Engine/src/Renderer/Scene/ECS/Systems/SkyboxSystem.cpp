#include "../../../../kitapch.h"
#include "SkyboxSystem.h"
#include <glm/ext/matrix_clip_space.hpp>

#include "../../../../Core/Engine.h"
#include "../Components/MeshComponent.h"
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

        auto entity = Entity(&scene, enttEntity);
        auto& [skyboxID, cubemapID] = entity.getComponent<SkyboxComponent>();
        auto& skyboxTexture = assetManager.getAsset<Texture>(skyboxID);

        if (m_skyboxToCubemapFBO == nullptr) {
            m_skyboxToCubemapFBO = FrameBuffer::createPtr();
            m_skyboxToCubemapFBO->createBuffer(CUBEMAP_FACE_RES,
                                               {{{BufferType::COLOR, FrameBuffer::AttachType::TEXTURE}, {BufferType::DEPTH, FrameBuffer::AttachType::TEXTURE}}},
                                               true, 1);
            m_skyboxID = skyboxID;

            renderer.setDepthFunc(DepthFunction::LEQUAL);
            renderer.disableBufferWrite(BufferType::DEPTH);
            renderer.setViewport(CUBEMAP_FACE_RES, false);
            m_skyboxToCubemapFBO->bind();

            auto& shader = assetManager.getOrCreateAsset<Shader>(std::nullopt, false,
                                                                 std::initializer_list{Shader::vert("KitaSkyboxBuilderVertex.glsl"), Shader::frag("KitaSkyboxBuilderFragment.glsl")});

            cubemapID = assetManager.createAsset<Texture>(std::nullopt, {}, Texture::TextureType::CUBEMAP, CUBEMAP_FACE_RES);
            auto& cubemapTexture = assetManager.getAsset<Texture>(cubemapID);
            auto cubeMeshID = assetManager.createAsset<Mesh>(std::nullopt, {}, Geometry::getCubeData());
            auto& cubeMesh = assetManager.getAsset<Mesh>(cubeMeshID);
            entity.addComponent<MeshComponent>(cubeMeshID);
            shader.bind();
            shader.setUniformMat4("projection", CubemapUtil::getCubemapCaptureProjection());
            const auto captureViews = CubemapUtil::getCubemapCaptureViews();
            for (int i = 0; i < 6; ++i) {
                m_skyboxToCubemapFBO->attachCubemapFace(cubemapTexture.getTexture(), i);

                shader.setUniformMat4("view", captureViews[i]);

                renderer.renderMesh(cubeMesh, shader, glm::mat4(1.0f), std::initializer_list{&skyboxTexture});
            }
            m_skyboxToCubemapFBO->unbind();
            renderer.restoreViewport();
            renderer.enableBufferWrite(BufferType::DEPTH);
            renderer.setDepthFunc(DepthFunction::LESS);
        }

        if (m_skyboxID != skyboxID) {
            m_skyboxID = skyboxID;
            m_skyboxToCubemapFBO->resize(skyboxTexture.getResolution());
        }

        renderer.setDepthFunc(DepthFunction::LEQUAL);
        renderer.disableBufferWrite(BufferType::DEPTH);

        auto& shader = assetManager.getOrCreateAsset<Shader>(std::nullopt, false,
                                                             std::initializer_list{Shader::vert("KitaSkyboxRenderVertex.glsl"), Shader::frag("KitaSkyboxRenderFragment.glsl")});
        renderer.renderMesh(assetManager.getAsset<Mesh>(entity.getComponent<MeshComponent>().meshID), shader, glm::mat4(1.0f), std::initializer_list{&skyboxTexture});

        renderer.enableBufferWrite(BufferType::DEPTH);
        renderer.setDepthFunc(DepthFunction::LESS);
    }
} // Kita
