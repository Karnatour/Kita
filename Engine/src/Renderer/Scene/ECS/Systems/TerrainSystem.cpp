#include "TerrainSystem.h"
#include "../Entity.h"
#include "../../Scene.h"
#include "../../../DebugScope.h"
#include "../../../../Core/Engine.h"
#include "../../../../Util/ColorUtil.h"
#include "../../../Util/CameraUtil.h"
#include "../Components/Components.h"

namespace Kita {
    int TerrainSystem::getOrder() {
        return Order::TERRAIN;
    }

    void TerrainSystem::update(Scene& scene) {
        KITA_ENGINE_PROFILE("Terrain system update");

        Entity cameraEntity(&scene, scene.view<CameraComponent, ActiveCamera>().front());
        const auto& cameraProperties = cameraEntity.getComponent<CameraComponent>().properties;
        const auto& position = cameraProperties.position;

        const bool firstFrame = Engine::getEngine()->isFirstFrame();
        const glm::vec2 camXZ(position.x, position.z);

        const int chunkX = static_cast<int>(std::floor(position.x / static_cast<float>(ChunkData::CHUNK_SIZE)));
        const int chunkZ = static_cast<int>(std::floor(position.z / static_cast<float>(ChunkData::CHUNK_SIZE)));
        const ChunkPos newChunkPos(chunkX, chunkZ);

        auto viewProjection = CameraUtil::getViewProjection(cameraProperties, Engine::getEngine()->getRenderer().getViewport());

        if (firstFrame) {
            m_quadTree = std::make_unique<QuadTree>(static_cast<float>(m_renderDistance * ChunkData::CHUNK_SIZE), LodQuality::HIGH);
            m_chunkRenderPool = std::make_unique<ChunkRenderPool>(m_quadTree->getMaxNodeCount());
            m_frustum = std::make_unique<Frustum>(viewProjection);
        }

        m_cameraPos = position;
        m_frustum->recalculate(viewProjection);

        processGeneratedNodes();
        swapIfReady();

        if (firstFrame || newChunkPos != m_cameraChunkPos) {
            m_cameraChunkPos = newChunkPos;
            updateNodes(camXZ);
            updateAllChunks();

            if (auto* environment = scene.contextGet<EnvironmentComponent>()) {
                auto& [fogColor, fogStart, fogEnd, fogDensity, fogMode] = environment->properties.fogProperties;
                fogEnd = static_cast<float>(m_renderDistance * ChunkData::CHUNK_SIZE);
                fogStart = 0.9f * fogEnd;
                fogDensity = 2.5f;
                fogColor = glm::vec3(0.5f, 0.6f, 1.0f);
                fogMode = FogMode::EXPONENTIAL2;
            }
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
            m_materialComponent.shaderID = assetManager.getOrCreateAsset<Shader>(AssetKey{"KitaTerrainVertex.glsl", "KitaTerrainFragment.glsl"}, {},
                                                                                 std::vector{Shader::vert(), Shader::frag({Shader::define("MAX_CASCADES", std::to_string(DirectionalShadowProperties::MAX_CASCADES))})}).id;
            m_materialComponent.albedoTextureID = assetManager.getOrCreateAsset<Texture>(AssetKey{"Ground047_2K-PNG/Ground047_2K-PNG_Color.png", "Rock023_2K-PNG/Rock023_2K-PNG_Color.png", "Snow008A_2K-PNG/Snow008A_2K-PNG_Color.png"}, {},
                                                                                         Texture::TextureType::ALBEDO, std::nullopt).id;
            m_materialComponent.metallicRoughnessTextureID = assetManager.getOrCreateAsset<Texture>(
                AssetKey{"Ground047_2K-PNG/Ground047_2K-PNG_Roughness.png", "Rock023_2K-PNG/Rock023_2K-PNG_Roughness.png", "Snow008A_2K-PNG/Snow008A_2K-PNG_Roughness.png"},
                {}, Texture::TextureType::METALLIC_ROUGHNESS, std::nullopt).id;
            m_materialComponent.normalTextureID = assetManager.getOrCreateAsset<Texture>(AssetKey{"Ground047_2K-PNG/Ground047_2K-PNG_NormalGL.png", "Rock023_2K-PNG/Rock023_2K-PNG_NormalGL.png", "Snow008A_2K-PNG/Snow008A_2K-PNG_NormalGL.png"}, {},
                                                                                         Texture::TextureType::NORMAL, std::nullopt).id;
            m_materialComponent.displacementTextureID = assetManager.getOrCreateAsset<Texture>(
                AssetKey{"Ground047_2K-PNG/Ground047_2K-PNG_Displacement.png", "Rock023_2K-PNG/Rock023_2K-PNG_Displacement.png", "Snow008A_2K-PNG/Snow008A_2K-PNG_Displacement.png"}, {},
                Texture::TextureType::DISPLACEMENT, std::nullopt).id;
            m_materialComponent.ambientOcclusionTextureID = assetManager.getOrCreateAsset<Texture>(
                AssetKey{"Ground047_2K-PNG/Ground047_2K-PNG_AmbientOcclusion.png", "Rock023_2K-PNG/Rock023_2K-PNG_AmbientOcclusion.png", "Snow008A_2K-PNG/Snow008A_2K-PNG_AmbientOcclusion.png"}, {}, Texture::TextureType::AMBIENT_OCCLUSION,
                std::nullopt).id;
        }

        Shader& shader = assetManager.getAsset<Shader>(m_materialComponent.shaderID);
        shader.bind();

        std::array<Texture*, 7> textures = {};
        textures[0] = &assetManager.getAsset<Texture>(m_materialComponent.albedoTextureID);
        textures[1] = &assetManager.getAsset<Texture>(m_materialComponent.metallicRoughnessTextureID);
        textures[2] = &assetManager.getAsset<Texture>(m_materialComponent.normalTextureID);
        textures[3] = &assetManager.getAsset<Texture>(m_materialComponent.displacementTextureID);
        textures[4] = &assetManager.getAsset<Texture>(m_materialComponent.ambientOcclusionTextureID);
        if (const Entity skyboxEntity(&scene, scene.view<SkyboxComponent>().front()); skyboxEntity) {
            if (const auto& skyboxCmp = skyboxEntity.getComponent<SkyboxComponent>(); skyboxCmp.irradianceCubemapID != AssetManager::INVALID_ASSET_ID) {
                textures[5] = &assetManager.getAsset<Texture>(skyboxCmp.irradianceCubemapID);
            }
        }

        if (const auto dirShadowComponent = scene.contextGet<DirectionalShadowComponent>()) {
            textures[6] = dirShadowComponent->properties.texture;
        }

        renderer.setMaterialInShader(shader, textures);
        shader.setUniformFloat("iblIntensity", scene.contextGet<SceneSettingsComponent>()->properties.iblIntensity); //TODO Move to UBO ?
        shader.setUniformMat4("model", glm::mat4(1.0f));

        m_chunkRenderPool->render(glm::vec2(m_cameraPos.x, m_cameraPos.z), *m_frustum);

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

    const Frustum& TerrainSystem::getFrustum() const {
        return *m_frustum;
    }

    ChunkRenderPool& TerrainSystem::getChunkRenderPool() const {
        return *m_chunkRenderPool;
    }

    void TerrainSystem::processGeneratedNodes() {
        int processed = 0;
        for (auto it = m_pendingNodeData.begin(); it != m_pendingNodeData.end();) {
            // ALLOWED_CHUNKS_PER_FRAME Prevents stuttering
            if (processed >= ALLOWED_CHUNKS_PER_FRAME || it->future.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                ++it;
                continue;
            }

            const NodeKey key = it->key;
            auto chunkData = it->future.get();

            it = m_pendingNodeData.erase(it);
            m_pendingNodes.erase(key);

            // Camera moved, nobody wants this node anymore so skip it
            if (!m_wantedKeys.contains(key)) {
                continue;
            }

            processed = processed + 1;

            const unsigned int slot = m_chunkRenderPool->addChunk(key, chunkData.getVertices());
            if (slot == ChunkRenderPool::INVALID_SLOT) {
                continue; // ChunkRenderPool is full
            }
            m_residentNodes[key] = slot;

            // Only LOD 0 nodes become simulated chunks
            if (key.lodLevel == 0) {
                Chunk chunk = m_worldGenerator.generateChunk(chunkData);
                const auto chunkPos = chunkData.getChunkPos();

                auto [newIt, inserted] = m_chunkMap.try_emplace(chunkPos, std::move(chunk));
                if (inserted) {
                    m_chunks.emplace_back(&newIt->second);
                    updateChunkState(newIt->second);
                }
            }
        }
    }

    void TerrainSystem::updateNodes(const glm::vec2 camXZ) {
        KITA_ENGINE_PROFILE("Update nodes");

        m_quadTree->update(camXZ, m_wantedNodes);

        // Sort so near terrain is generated before far terrain
        std::ranges::sort(m_wantedNodes, [&](const QuadNode* a, const QuadNode* b) {
            return a->getAABB().getDistanceSq(camXZ) < b->getAABB().getDistanceSq(camXZ);
        });

        m_wantedKeys.clear();
        for (const QuadNode* node : m_wantedNodes) {
            const NodeKey key = node->getKey();
            m_wantedKeys.insert(key);

            // Skip if Chunk is already uploaded in GPU or is already being generated
            if (m_residentNodes.contains(key) || m_pendingNodes.contains(key)) {
                continue;
            }

            // Submit task
            m_pendingNodes.insert(key);
            m_pendingNodeData.push_back(PendingNode{.key = key, .future = Engine::getEngine()->getThreadPool().submit(Task::Priority::NORMAL, [this, key] { return m_worldGenerator.generateChunkData(key); })});
        }
    }

    void TerrainSystem::swapIfReady() {
        for (const NodeKey& key : m_wantedKeys) {
            if (!m_residentNodes.contains(key)) {
                return; // Waiting for generation/upload to ChunkRenderPool, to prevent holes
            }
        }

        // Render new chunks
        for (const auto& [key, slot] : m_residentNodes) {
            if (m_wantedKeys.contains(key)) {
                m_chunkRenderPool->show(slot);
            }
        }

        // Release swapped chunks
        std::erase_if(m_residentNodes, [&](const std::pair<const NodeKey, unsigned int>& entry) {
            if (m_wantedKeys.contains(entry.first)) {
                return false;
            }
            m_chunkRenderPool->release(entry.second);
            return true;
        });
    }

    void TerrainSystem::updateAllChunks() {
        for (auto* chunk : m_chunks) {
            updateChunkState(*chunk);
        }
    }

    void TerrainSystem::updateChunkState(Chunk& chunk) {
        KITA_ENGINE_PROFILE("Update chunk state");
        const ChunkPos chunkPos = chunk.getChunkData().getChunkPos();
        chunkPos.isChunkInRadius(m_cameraChunkPos, m_simulationDistance) ? chunk.enableSimulation() : chunk.disableSimulation();
    }
} // Kita
