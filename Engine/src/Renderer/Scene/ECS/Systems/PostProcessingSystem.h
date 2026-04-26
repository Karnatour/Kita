#pragma once
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    class KITAENGINE_API PostProcessingSystem : public System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        AssetManager::AssetID m_quadMeshAssetID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_shaderAssetID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
