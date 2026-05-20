#pragma once
#include <glm/glm.hpp>
#include "System.h"
#include "../../../../Core/DllTemplate.h"
#include "../../../Buffers/ShaderStorageBuffer.h"
#include "../../../Buffers/UniformBuffer.h"
#include "../../../Properties/LightShadowProperties.h"
#include "../Entity.h"
#include "../../../../Assets/AssetManager.h"
#include "../../../Buffers/FrameBuffer.h"

namespace Kita {
    class KITAENGINE_API LightShadowSystem final : public System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        // std430 layout
        struct LightSSBOLayout {
            static constexpr int INITIAL_LIGHT_CAPACITY_COUNT = 128;

            glm::vec4 position = glm::vec4(0.0f, 0.0f, 3.0f, 0.0f);
            glm::vec4 direction = glm::vec4(0.0f, 0.0f, 3.0f, 0.0f);
            glm::vec4 diffuse = glm::vec4(0.8f, 0.75f, 0.7f, 0.0f);
            glm::vec4 cutOff = glm::vec4(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)), 0.0f, 0.0f);
            glm::ivec4 params = glm::ivec4(
                -1, // lightType
                -1, // castsShadows
                -1, // pointCubemapArray index
                -1 //  unused
            );
        };


        // std140 layout
        struct alignas(16) DirectionalShadowUBOLayout {
            static constexpr int MAX_CASCADES = 16;

            glm::mat4 lightSpaceMatrices[MAX_CASCADES];
            glm::vec4 cascadeSplitDistances[MAX_CASCADES]; //x stores the distance, rest is padding
            glm::ivec4 params = glm::ivec4(
                -1, // x = cascadeCount
                -1, // y = unused
                -1, // z = unused
                -1 // w = unused
            );
        };

        void createBuffersIfMissing(Scene& scene);
        void renderShadowPass(Scene& scene);
        void uploadLightData(Scene& scene);
        void uploadDirectionalLightData(Scene& scene) const;
        LightSSBOLayout convertLightToSSBOLayout(Entity entity, const LightProperties& properties);

        std::unique_ptr<ShaderStorageBuffer> m_lightSSBO;
        std::unique_ptr<UniformBuffer> m_directionalShadowUBO;
        std::unique_ptr<FrameBuffer> m_lightFBO;
        int m_castsShadowsCount = 0;
        int m_cascadeCount = 0;
        AssetManager::AssetID m_CSMShaderAssetID = AssetManager::INVALID_ASSET_ID;
    };
} // Kita
