#pragma once
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../../../../Assets/AssetManager.h"
#include "../../../../Events/RendererEvents.h"
#include "../../../Buffers/FrameBuffer.h"
#include "../../../Buffers/UniformBuffer.h"

namespace Kita {
    class KITAENGINE_API EnvironmentSystem : public System {
    public:
        explicit EnvironmentSystem(Scene& scene)
            : System(scene) {
        }

        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        void onViewportResized(const ViewportResized& event);

        // std140 layout
        struct alignas(16) EnvironmentUBOLayout {
            glm::vec4 fogColor;
            glm::vec4 fogParams = glm::vec4(
                0.0f, // fogStart
                0.0f, // fogEnd
                0.0f, // fogDensity
                0.0f // fogMode
            );
        };

        std::unique_ptr<UniformBuffer> m_environmentUBO;
        std::unique_ptr<FrameBuffer> m_environmentalFBO;


        AssetManager::AssetID m_quadMeshAssetID = AssetManager::INVALID_ASSET_ID;
        AssetManager::AssetID m_shaderAssetID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
