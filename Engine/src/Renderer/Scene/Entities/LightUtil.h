#pragma once
#include "../../../Core/DllTemplate.h"
#include "../../Buffers/FrameBuffer.h"
#include <glm/glm.hpp>

namespace Kita {
    struct LightProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 attenuation = glm::vec3(1.0f, 0.09f, 0.032f);
        glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.35f);
        glm::vec3 diffuse = glm::vec3(0.8f, 0.75f, 0.7f);
        glm::vec3 specular = glm::vec3(0.4f, 0.35f, 0.3f);
        glm::vec2 cutOff = glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
        glm::mat4 lightSpaceMatrix = 1.0f;
        int lightType;
    };

    struct ShadowProperties {
        static constexpr std::pair<int, int> resolution = std::make_pair(4096, 4096);
        std::unique_ptr<FrameBuffer> depthMapFBO = FrameBuffer::createPtr();
    };

    class KITAENGINE_API LightUtil {
    public:
        enum class LightType {
            DIRECTIONAL = 0,
            POINT = 1,
            SPOT = 2
        };

        //16 because of std430 rules;
        static constexpr int MAX_LIGHTS = 512;

        bool onRender(RendererAPI& rendererApi) override;

        void beginShadowMapRender(RendererAPI& rendererApi);
        void endShadowMapRender(RendererAPI& rendererApi);

    private:
        void prepareLightModel();
    };
} // Kita
