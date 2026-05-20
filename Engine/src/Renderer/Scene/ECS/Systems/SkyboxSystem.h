#pragma once
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../../../../Assets/AssetManager.h"
#include "../../../Buffers/FrameBuffer.h"
#include "../Components/SkyboxComponent.h"

namespace Kita {
    class KITAENGINE_API SkyboxSystem : public System{
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        static constexpr std::pair<int,int> CUBEMAP_FACE_RES = std::make_pair(2048,2048);
        static constexpr std::pair<int,int> IRRADIANCE_RES = std::make_pair(32,32);
        void convertSkyboxTextureToCubemap(SkyboxComponent& skyboxComponent);
        void convertCubemapToIrradance(SkyboxComponent& skyboxComponent);
        void createAssetsIfNotExists();

        std::unique_ptr<FrameBuffer> m_cubemapCaptureFBO;
        AssetManager::AssetID m_skyboxTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxMeshID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxBuilderShaderID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxRenderShaderID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxBuilderIrradianceShaderID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
