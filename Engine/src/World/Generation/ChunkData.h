#pragma once
#include <vector>
#include <array>

#include "../ChunkPos.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

namespace Kita {
    class ChunkData {
    public:
        static constexpr int CHUNK_SIZE = 16;
        static constexpr int CHUNK_QUAD_PER_EDGE = 16;
        static constexpr int HEIGHTMAP_EDGE = CHUNK_QUAD_PER_EDGE + 3;
        static constexpr int HEIGHTMAP_SIZE = HEIGHTMAP_EDGE * HEIGHTMAP_EDGE;

        ChunkData(const ChunkPos& chunkPos, std::vector<std::byte> vertices, std::array<float, HEIGHTMAP_SIZE> heightmap, std::optional<JPH::BodyCreationSettings> bodyCreationSettings, std::pair<float,float> minMaxY);

        ChunkPos getChunkPos() const;
        const std::vector<std::byte>& getVertices() const;
        const std::optional<JPH::BodyCreationSettings>& getBodyCreationSettings() const;
        const std::array<float, HEIGHTMAP_SIZE>& getHeightmap() const;
        std::pair<float,float> getMinMaxY() const;

        void setBodyID(JPH::BodyID bodyID);
        JPH::BodyID getBodyID() const;

    private:
        ChunkPos m_chunkPos;
        std::vector<std::byte> m_vertices;
        std::array<float, HEIGHTMAP_SIZE> m_heightmap;
        std::pair<float,float> m_minMaxY;

        std::optional<JPH::BodyCreationSettings> m_bodySettings;
        JPH::BodyID m_bodyID = JPH::BodyID(JPH::BodyID::cInvalidBodyID);
    };
} // Kita
