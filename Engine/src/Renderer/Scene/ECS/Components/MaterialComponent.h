#pragma once
#include "../../../../Assets/AssetManager.h"
#include "../../../Properties/PhongProperties.h"

namespace Kita {
    struct MaterialComponent {
        AssetManager::AssetID shaderID = AssetManager::DEFAULT_ASSET_ID;

        AssetManager::AssetID diffuseTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID specularTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID normalTtextureID = AssetManager::INVALID_ASSET_ID;

        PhongProperties properties;
    };

} //Kita
