#include "TerrainSystem.h"
#include "../Entity.h"
#include "../../Scene.h"
#include "../../../../Core/Engine.h"
#include "../Components/Components.h"

namespace Kita {
    int TerrainSystem::getOrder() {
        return Order::TERRAIN;
    }

    void TerrainSystem::update(Scene& scene) {
        Entity cameraEntity(&scene, scene.view<CameraComponent, ActiveCamera>().front());

        const auto& position = cameraEntity.getComponent<CameraComponent>().properties.position;

        const int chunkX = static_cast<int>(std::floor(position.x / static_cast<float>(Chunk::CHUNK_SIZE)));

        const int chunkZ = static_cast<int>(std::floor(position.z / static_cast<float>(Chunk::CHUNK_SIZE)));

        const ChunkPos newChunkPos(chunkX, chunkZ);
        if (newChunkPos == m_cameraChunkPos && !Engine::getEngine()->isFirstFrame()) {
            return;
        }

        for (int x = newChunkPos.getChunkX() - m_renderDistance; x <= newChunkPos.getChunkX() + m_renderDistance; ++x) {
            for (int z = newChunkPos.getChunkZ() - m_renderDistance; z <= newChunkPos.getChunkZ() + m_renderDistance; ++z) {
                ChunkPos chunkPos(x, z);


                auto it = m_chunkMap.find(chunkPos);
                if (it == m_chunkMap.end() || m_pendingChunkPos.contains(chunkPos)) {
                    m_pendingChunkPos.emplace(chunkPos);
                    m_pendingChunkMeshData.emplace_back(Engine::getEngine()->getThreadPool().submit(Task::Priority::NORMAL, [this, chunkPos] { return m_worldGenerator.generateChunkMeshData(chunkPos); }));
                }
            }
        }

        for (auto it = m_pendingChunkMeshData.begin(); it != m_pendingChunkMeshData.end();) {
            if (it->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                auto meshData = it->get();
                Chunk chunk = m_worldGenerator.generateChunk(meshData);
                m_chunkMap.try_emplace(meshData.getChunkPos(), std::move(chunk));
                it = m_pendingChunkMeshData.erase(it);
            } else ++it;
        }

        m_cameraChunkPos = newChunkPos;
    }

    void TerrainSystem::render(Scene& scene) {
        auto& assetManager = Engine::getEngine()->getAssetManager();
        auto& renderer = Engine::getEngine()->getRenderer();
        renderer.getMainFramebuffer().bind();

        if (m_materialComponent.albedoTextureID == AssetManager::INVALID_ASSET_ID) {
            m_materialComponent.albedoTextureID = assetManager.createAsset<Texture>("Rock029_2K-PNG_Color.png", {}, Texture::TextureType::ALBEDO, std::nullopt);
            m_materialComponent.metallicRoughnessTextureID = assetManager.createAsset<Texture>("Rock029_2K-PNG_Roughness.png", {}, Texture::TextureType::METALLIC_ROUGHNESS, std::nullopt);
            m_materialComponent.normalTextureID = assetManager.createAsset<Texture>("Rock029_2K-PNG_NormalGL.png", {}, Texture::TextureType::NORMAL, std::nullopt);
        }

        Shader& shader = assetManager.getAsset<Shader>(m_materialComponent.shaderID);
        shader.bind();

        std::array<Texture*, 5> textures = {};
        textures[0] = &assetManager.getAsset<Texture>(m_materialComponent.albedoTextureID);
        //textures[1] = &assetManager.getAsset<Texture>(m_materialComponent.metallicRoughnessTextureID);
        textures[2] = &assetManager.getAsset<Texture>(m_materialComponent.normalTextureID);
        if (const Entity skyboxEntity(&scene, scene.view<SkyboxComponent>().front()); skyboxEntity) {
            if (const auto& skyboxCmp = skyboxEntity.getComponent<SkyboxComponent>(); skyboxCmp.irradianceCubemapID != AssetManager::INVALID_ASSET_ID) {
                textures[3] = &assetManager.getAsset<Texture>(skyboxCmp.irradianceCubemapID);
            }
        }

        if (const Entity dirShadowEntity(&scene, scene.view<DirectionalShadowComponent>().front()); dirShadowEntity) {
            textures[4] = dirShadowEntity.getComponent<DirectionalShadowComponent>().properties.texture;
        }

        for (int x = m_cameraChunkPos.getChunkX() - m_renderDistance; x <= m_cameraChunkPos.getChunkX() + m_renderDistance; ++x) {
            for (int z = m_cameraChunkPos.getChunkZ() - m_renderDistance; z <= m_cameraChunkPos.getChunkZ() + m_renderDistance; ++z) {
                ChunkPos chunkPos(x, z);

                auto it = m_chunkMap.find(chunkPos);
                if (it == m_chunkMap.end()) {
                    // The chunks is still generating
                    return;
                }

                auto& chunk = it->second;
                Mesh& mesh = assetManager.getAsset<Mesh>(chunk.getMeshAssetID());

                shader.setUniformFloat("iblIntensity", Entity(&scene, scene.view<SceneSettingsComponent>().front()).getComponent<SceneSettingsComponent>().properties.iblIntensity); //TODO Move to UBO ?
                renderer.renderMesh(mesh, shader, glm::mat4(1.0f), textures);
            }
        }

        renderer.getMainFramebuffer().unbind();
    }

    void TerrainSystem::setRenderDistance(const int renderDistance) {
        m_renderDistance = renderDistance;
    }

    int TerrainSystem::getRenderDistance() {
        return m_renderDistance;
    }
} // Kita
