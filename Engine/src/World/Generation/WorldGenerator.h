#pragma once
#include "ChunkMeshData.h"
#include "../../Core/DllTemplate.h"
#include "../Chunk.h"
#include "FastNoise/Generators/Fractal.h"
#include "FastNoise/Utility/SmartNode.h"

namespace Kita {
    class KITAENGINE_API WorldGenerator {
    public:
        WorldGenerator(int octaveCount, float gain, float lacunarity);
        ChunkMeshData generateChunkMeshData(const ChunkPos& chunkPos);
        Chunk generateChunk(ChunkMeshData chunkMeshData);
    private:
        int m_octaveCount;
        int m_gain;
        int m_lacunarity;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_noise;
    };
} // Kita
