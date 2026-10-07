#pragma once
#include "../../Core/DllTemplate.h"
#include "FastNoise/FastNoise.h"

namespace Kita {
    struct KITAENGINE_API Noise {
        static FastNoise::SmartNode<FastNoise::FractalFBm> getTerrainNoise(int octaveCount, int gain, int lacunarity);
    };
} // Kita
