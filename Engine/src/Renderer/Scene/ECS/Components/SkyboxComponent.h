#pragma once
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    struct SkyboxComponent {
        AssetManager::AssetID skyboxID;
        AssetManager::AssetID cubemapID;
    };
} // Kita
