#include "Noise.h"

namespace Kita {
    FastNoise::SmartNode<> Noise::getTerrainNoise(const int octaveCount, const float gain, const float lacunarity) {
        auto simplex = FastNoise::New<FastNoise::Simplex>();
        simplex->SetScale(1500.0f);

        auto fractalFBm = FastNoise::New<FastNoise::FractalFBm>();
        fractalFBm->SetSource(simplex);
        fractalFBm->SetOctaveCount(8);
        fractalFBm->SetLacunarity(2.0f);
        fractalFBm->SetWeightedStrength(0.0f);

        auto domainWarpGradient = FastNoise::New<FastNoise::DomainWarpGradient>();
        domainWarpGradient->SetSource(fractalFBm);
        domainWarpGradient->SetScale(100.0f);
        domainWarpGradient->SetWarpAmplitude(50.0f);

        auto remap = FastNoise::New<FastNoise::Remap>();
        remap->SetSource(domainWarpGradient);
        remap->SetFromMin(-1.0f);
        remap->SetFromMax(1.0f);
        remap->SetToMin(0.0f);
        remap->SetToMax(1.0f);

        auto powFloat = FastNoise::New<FastNoise::PowInt>();
        powFloat->SetValue(remap);
        powFloat->SetPow(4.0f);

        return powFloat;
    }
} // Kita
