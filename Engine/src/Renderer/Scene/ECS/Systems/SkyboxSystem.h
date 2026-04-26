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
        static constexpr std::pair<int,int> CUBEMAP_FACE_RES = std::make_pair(1024,1024);
        void convertSkyboxTextureToCubemap(SkyboxComponent& skyboxComponent);

        std::unique_ptr<FrameBuffer> m_skyboxToCubemapFBO;
        AssetManager::AssetID m_skyboxTextureID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxMeshID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxBuilderShaderID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_skyboxRenderShaderID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
