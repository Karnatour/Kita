#pragma once
#include "../../../Assets/AssetManager.h"

namespace Kita {
    struct MaterialComponent {
        AssetManager::AssetID m_shader;
        AssetManager::AssetID m_diffuseTexture;
        AssetManager::AssetID m_specularTexture;
        AssetManager::AssetID m_normalTexture;
    };
} //Kita
