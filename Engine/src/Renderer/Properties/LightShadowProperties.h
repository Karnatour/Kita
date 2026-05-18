#pragma once
#include <array>
#include <glm/glm.hpp>

#include "../../Assets/Texture.h"
#include "../Util/LightUtil.h"

namespace Kita {
    struct LightProperties {
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 attenuation = glm::vec3(1.0f, 0.09f, 0.032f);
        glm::vec3 ambient = glm::vec3(0.3f, 0.3f, 0.35f);
        glm::vec3 diffuse = glm::vec3(0.8f, 0.75f, 0.7f);
        glm::vec3 specular = glm::vec3(0.4f, 0.35f, 0.3f);
        glm::vec2 cutOff = glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)));
        LightType lightType = LightType::SPOT;
    };

    struct DirectionalShadowProperties {
        static constexpr int MAX_CASCADES = 16;

        std::array<float, MAX_CASCADES> cascadeSplitDistances{};
        std::array<glm::mat4, MAX_CASCADES> lightSpaceMatrices{};
        std::pair<int, int> resolution = std::make_pair(4096, 4096);
        Texture* texture = nullptr;
        int cascadeCount = 4;
    };


}
