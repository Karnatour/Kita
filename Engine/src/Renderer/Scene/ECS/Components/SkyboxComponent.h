#pragma once
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    // Only skyboxID must be provided, SkyboxSystem will create cubemapID
    struct SkyboxComponent {
        AssetManager::AssetID skyboxID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID cubemapID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
