#pragma once
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    struct MaterialComponent {
        AssetManager::AssetID shaderID = AssetManager::DEFAULT_ASSET_ID;

        AssetManager::AssetID albedoTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID metallicRoughnessTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID normalTextureID = AssetManager::INVALID_ASSET_ID;
    };

} //Kita
