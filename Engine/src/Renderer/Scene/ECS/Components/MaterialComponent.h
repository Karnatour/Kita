#pragma once
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    struct ShaderComponent {
        AssetManager::AssetID shaderID;
    };

    struct DiffuseMapComponent {
        AssetManager::AssetID textureID;
    };

    struct NormalMapComponent {
        AssetManager::AssetID textureID;
    };

    struct SpecularMapComponent {
        AssetManager::AssetID textureID;
    };
} //Kita
