#pragma once
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    struct ShaderComponent {
        AssetManager::AssetID shaderID = AssetManager::DEFAULT_ASSET_ID;
    };

    struct DiffuseMapComponent {
        AssetManager::AssetID textureID = AssetManager::DEFAULT_ASSET_ID;
    };

    struct SpecularMapComponent {
        AssetManager::AssetID textureID = AssetManager::DEFAULT_ASSET_ID;
    };

    struct NormalMapComponent {
        AssetManager::AssetID textureID = AssetManager::DEFAULT_ASSET_ID;
    };
} //Kita
