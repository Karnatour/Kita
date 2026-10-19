#include "TerrainSystem.h"
#include "../Entity.h"
#include "../../Scene.h"
#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../../../../Util/ColorUtil.h"
#include "../Components/Components.h"

namespace Kita {
    int TerrainSystem::getOrder() {
        return Order::TERRAIN;
    }

    void TerrainSystem::update(Scene& scene) {
        KITA_ENGINE_PROFILE("Terrain system update");

        if (Engine::getEngine()->isFirstFrame()) {
            m_chunkRenderPool = std::make_unique<ChunkRenderPool>(m_renderDistance);
        }

        Entity cameraEntity(&scene, scene.view<CameraComponent, ActiveCamera>().front());

        const auto& position = cameraEntity.getComponent<CameraComponent>().properties.position;

        const int chunkX = static_cast<int>(std::floor(position.x / static_cast<float>(ChunkData::CHUNK_SIZE)));
        const int chunkZ = static_cast<int>(std::floor(position.z / static_cast<float>(ChunkData::CHUNK_SIZE)));

        const ChunkPos newChunkPos(chunkX, chunkZ);
        processGeneratedChunkMeshData();

        if (newChunkPos == m_cameraChunkPos && !Engine::getEngine()->isFirstFrame()) {
            return;
        }

        m_cameraChunkPos = newChunkPos;

        updateAllChunks();
        generateChunkMeshData();

        if (auto* environment = scene.contextGet<EnvironmentComponent>()) {
            auto& fogProperties = environment->properties.fogProperties;
            fogProperties.fogStart = 0.85f * m_renderDistance * ChunkData::CHUNK_SIZE;
            fogProperties.fogEnd = 1.0f * m_renderDistance * ChunkData::CHUNK_SIZE;
            fogProperties.fogDensity = 2.5f;
            fogProperties.fogColor = glm::vec3(0.429f, 0.185f, 0.112f);
            fogProperties.fogMode = FogMode::EXPONENTIAL2;
        }
    }

    void TerrainSystem::render(Scene& scene) {
        KITA_ENGINE_PROFILE("Terrain system render");
        const auto debugScope = DebugScope::createPtr();
        debugScope->scope("Terrain pass");

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

        renderer.setMaterialInShader(shader, textures);
        shader.setUniformFloat("iblIntensity", scene.contextGet<SceneSettingsComponent>()->properties.iblIntensity); //TODO Move to UBO ?
        shader.setUniformMat4("model", glm::mat4(1.0f));
        m_chunkRenderPool->render();

        renderer.getMainFramebuffer().unbind();
    }

    void TerrainSystem::setRenderDistance(const int renderDistance) {
        m_renderDistance = renderDistance;
    }

    int TerrainSystem::getRenderDistance() const {
        return m_renderDistance;
    }

    const Chunk* TerrainSystem::getChunkForPos(const ChunkPos& chunkPos) const {
        const auto it = m_chunkMap.find(chunkPos);

        if (it == m_chunkMap.end()) {
            return nullptr;
        }

        return &it->second;
    }

    const Chunk* TerrainSystem::getCameraChunk() const {
        return getChunkForPos(m_cameraChunkPos);
    }

    ChunkPos TerrainSystem::getCameraChunkPos() const {
        return m_cameraChunkPos;
    }

    const std::unique_ptr<ChunkRenderPool>& TerrainSystem::getChunkRenderPool() const {
        return m_chunkRenderPool;
    }

    void TerrainSystem::generateChunkMeshData() {
        const int centerX = m_cameraChunkPos.getChunkX();
        const int centerZ = m_cameraChunkPos.getChunkZ();

        struct Candidate {
            ChunkPos pos;
            int distSq;
        };

        std::vector<Candidate> candidates;
        candidates.reserve((2 * m_renderDistance + 1) * (2 * m_renderDistance + 1));

        for (int x = centerX - m_renderDistance; x <= centerX + m_renderDistance; ++x) {
            for (int z = centerZ - m_renderDistance; z <= centerZ + m_renderDistance; ++z) {
                ChunkPos chunkPos(x, z);

                if (m_chunkMap.contains(chunkPos) || m_pendingChunkPos.contains(chunkPos)) {
                    continue;
                }

                const int dx = x - centerX;
                const int dz = z - centerZ;
                candidates.emplace_back(chunkPos, dx * dx + dz * dz);
            }
        }

        // Closest to the center chunk first
        std::ranges::sort(candidates, [](const Candidate& a, const Candidate& b) { return a.distSq < b.distSq; });

        for (const auto& [pos, distSq] : candidates) {
            const ChunkPos chunkPos = pos;
            m_pendingChunkPos.emplace(chunkPos);
            m_pendingChunkMeshData.emplace_back(Engine::getEngine()->getThreadPool().submit(Task::Priority::NORMAL, [this, chunkPos] { return m_worldGenerator.generateChunkData(chunkPos); }));
        }
    }

    void TerrainSystem::processGeneratedChunkMeshData() {
        int processedChunks = 0;
        for (auto pendingIt = m_pendingChunkMeshData.begin(); pendingIt != m_pendingChunkMeshData.end();) {
            // ALLOWED_CHUNKS_PER_FRAME Prevents stuttering
            if (processedChunks < ALLOWED_CHUNKS_PER_FRAME && pendingIt->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                auto chunkData = pendingIt->get();

                Chunk chunk = m_worldGenerator.generateChunk(chunkData);
                auto chunkPos = chunkData.getChunkPos();

                auto [newIt, inserted] = m_chunkMap.try_emplace(chunkPos, std::move(chunk));
                if (inserted) {
                    m_chunks.emplace_back(&newIt->second);
                    updateChunkState(newIt->second);
                }

                m_pendingChunkPos.erase(chunkPos);
                pendingIt = m_pendingChunkMeshData.erase(pendingIt);
                processedChunks = processedChunks + 1;
            } else {
                ++pendingIt;
            }
        }
    }

    void TerrainSystem::updateAllChunks() {
        // First clear slots outside of render distance before adding new chunks to the pool
        for (auto* chunk : m_chunks) {
            const ChunkPos pos = chunk->getChunkData().getChunkPos();
            if (chunk->hasValidSlot() && !pos.isChunkInRadius(m_cameraChunkPos, m_renderDistance)) {
                m_chunkRenderPool->release(chunk->getSlot());
                chunk->clearSlot();
            }
        }

        for (auto* chunk : m_chunks) {
            updateChunkState(*chunk);
        }
    }

    void TerrainSystem::updateChunkState(Chunk& chunk) {
        KITA_ENGINE_PROFILE("Update chunk state");
        const ChunkPos chunkPos = chunk.getChunkData().getChunkPos();
        const bool inRenderRange = chunkPos.isChunkInRadius(m_cameraChunkPos, m_renderDistance);
        const bool inSimulationRange = chunkPos.isChunkInRadius(m_cameraChunkPos, m_simulationDistance);

        if (inRenderRange && !chunk.hasValidSlot()) {
            chunk.setSlot(m_chunkRenderPool->addChunk(chunk.getChunkData().getVertices()));
        } else if (!inRenderRange && chunk.hasValidSlot()) {
            m_chunkRenderPool->release(chunk.getSlot());
            chunk.clearSlot();
        }

        inSimulationRange ? chunk.enableSimulation() : chunk.disableSimulation();
    }
} // Kita
