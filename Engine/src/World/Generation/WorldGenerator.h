#pragma once
#include "ChunkData.h"
#include "../../Core/DllTemplate.h"
#include "../Chunk.h"
#include "FastNoise/Generators/Fractal.h"
#include "FastNoise/Utility/SmartNode.h"

namespace Kita {
    class KITAENGINE_API WorldGenerator {
    public:
        //TODO As params
        static constexpr int SEED = 69420;

        static constexpr int VERTICES_PER_EDGE = ChunkData::CHUNK_QUAD_PER_EDGE + 1; // +1 -> One more vertex than quads because N quads need N+1 vertices
        static constexpr int SAMPLES_PER_EDGE = VERTICES_PER_EDGE + 2; //  +2 -> Normals need neighbours, so we need one extra sample on each side
        static constexpr int PHYSICS_SAMPLES = VERTICES_PER_EDGE + (VERTICES_PER_EDGE % 2); // Must be even number

        static constexpr float HEIGHT_SCALE = 250.0f; // Used for converting noise values (-1,1) to meters
        static constexpr float NOISE_FREQUENCY = 1.0f / 64.0f; // Lower = bigger hills. This needs to be power of two otherwise will cause ultra small holes
        WorldGenerator(int octaveCount, float gain, float lacunarity);
        ChunkData generateChunkData(const ChunkPos& chunkPos);
        Chunk generateChunk(ChunkData chunkData);
    private:
        int m_octaveCount;
        int m_gain;
        int m_lacunarity;
        FastNoise::SmartNode<> m_noise;
    };
} // Kita
