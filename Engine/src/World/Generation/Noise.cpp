#include "Noise.h"

namespace Kita {
    FastNoise::SmartNode<FastNoise::FractalFBm> Noise::getTerrainNoise(const int octaveCount, const float gain, const float lacunarity) {
        auto fbm = FastNoise::New<FastNoise::FractalFBm>();
        fbm->SetSource(FastNoise::New<FastNoise::SuperSimplex>());
        fbm->SetOctaveCount(octaveCount);
        fbm->SetGain(gain);
        fbm->SetLacunarity(lacunarity);

        return fbm;
    }
} // Kita
