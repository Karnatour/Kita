#pragma once
#include "../../Core/DllTemplate.h"
#include "../Chunk.h"
#include "FastNoise/Generators/Fractal.h"
#include "FastNoise/Utility/SmartNode.h"

namespace Kita {
    class KITAENGINE_API WorldGenerator {
    public:
        WorldGenerator(int octaveCount, int gain, int lacunarity);
        Chunk generateChunk(const ChunkPos& chunkPos);
    private:
        int m_octaveCount;
        int m_gain;
        int m_lacunarity;
        FastNoise::SmartNode<FastNoise::FractalFBm> m_noise;
    };
} // Kita
