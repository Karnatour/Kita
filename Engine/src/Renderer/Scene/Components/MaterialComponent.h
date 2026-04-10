#pragma once
#include "../../../Assets/AssetManager.h"

namespace Kita {
    struct MaterialComponent {
        AssetManager::AssetID shader;
        AssetManager::AssetID diffuseTexture;
        AssetManager::AssetID specularTexture;
        AssetManager::AssetID normalTexture;
    };
} //Kita
