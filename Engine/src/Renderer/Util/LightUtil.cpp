#include "../../kitapch.h"
#include "LightUtil.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "CameraUtil.h"
#include "../RendererAPI.h"
#include "../../Core/Engine.h"

namespace Kita {
    std::vector<glm::vec4> LightUtil::getFrustumPoints(const glm::mat4& view, const glm::mat4& projection) {
        const auto inv = glm::inverse(projection * view);

        std::vector<glm::vec4> frustumCorners;
        for (unsigned int x = 0; x < 2; ++x) {
            for (unsigned int y = 0; y < 2; ++y) {
                for (unsigned int z = 0; z < 2; ++z) {
                    const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.emplace_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    glm::mat4 LightUtil::getLightSpaceMatrix(const CameraProperties& properties, const float zNear, const float zFar, const glm::vec3& lightDir, const std::pair<int, int> viewportResolution,
                                             const std::pair<int, int> shadowMapResolution) {
        const glm::mat4 projection = glm::perspective(glm::radians(properties.fov), static_cast<float>(viewportResolution.first) / static_cast<float>(viewportResolution.second), zNear, zFar);
        const std::vector<glm::vec4> corners = getFrustumPoints(CameraUtil::getViewMatrix(properties), projection);

        auto center = glm::vec3(0.0f);
        for (const auto& corner : corners) {
            center += glm::vec3(corner);
        }
        center /= static_cast<float>(corners.size());

        const float radius = glm::length(corners.front() - corners.back()) / 2.0f;
        const float unitsPerTexel = static_cast<float>(shadowMapResolution.first) / (radius * 2.0f);

        auto scalar = glm::mat4(1.0f);
        scalar = glm::scale(scalar, glm::vec3(unitsPerTexel));

        glm::mat4 baseView = glm::lookAtLH(glm::vec3(0.0f, 0.0f, 0.0f), -lightDir, glm::vec3(0.0f, 1.0f, 0.0f));
        baseView = baseView * scalar;
        const glm::mat4 baseViewInv = glm::inverse(baseView);

        center = glm::vec3(baseView * glm::vec4(center, 1.0f));
        center.x = std::floor(center.x);
        center.y = std::floor(center.y);
        center = glm::vec3(baseViewInv * glm::vec4(center, 1.0f));

        const glm::vec3 eye = center - (lightDir * radius * 2.0f);

        const glm::mat4 lightView = glm::lookAtLH(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));

        constexpr float zMult = 6.0f; //TODO Config
        const glm::mat4 lightProjection = glm::orthoLH(-radius, radius, -radius, radius, -radius * zMult, radius * zMult);

        return lightProjection * lightView;
    }
} // Kita
