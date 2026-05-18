#include "../../kitapch.h"
#include "LightUtil.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "CameraUtil.h"
#include "../RendererAPI.h"
#include "../../Core/Engine.h"

namespace Kita {
    std::vector<glm::vec4> LightUtil::getFrustrumPoints(const glm::mat4& view, const glm::mat4& projection) {
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
        const std::vector<glm::vec4> corners = getFrustrumPoints(CameraUtil::getViewMatrix(properties), projection);

        auto center = glm::vec3(0.0f);
        for (const auto& corner : corners) {
            center += glm::vec3(corner);
        }
        center /= static_cast<float>(corners.size());

        const glm::mat4 lightView = glm::lookAt(center + glm::normalize(-lightDir), center, glm::vec3(0.0f, 1.0f, 0.0f));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();

        for (const auto& corner : corners) {
            const glm::vec4 lightSpace = lightView * corner;
            minX = std::min(minX, lightSpace.x);
            maxX = std::max(maxX, lightSpace.x);
            minY = std::min(minY, lightSpace.y);
            maxY = std::max(maxY, lightSpace.y);
            minZ = std::min(minZ, lightSpace.z);
            maxZ = std::max(maxZ, lightSpace.z);
        }

        const float worldSizeX = maxX - minX;
        const float worldSizeY = maxY - minY;
        const float unitsPerTexelX = worldSizeX / static_cast<float>(shadowMapResolution.first);
        const float unitsPerTexelY = worldSizeY / static_cast<float>(shadowMapResolution.second);

        minX = std::floor(minX / unitsPerTexelX) * unitsPerTexelX;
        maxX = std::floor(maxX / unitsPerTexelX) * unitsPerTexelX;
        minY = std::floor(minY / unitsPerTexelY) * unitsPerTexelY;
        maxY = std::floor(maxY / unitsPerTexelY) * unitsPerTexelY;

        constexpr float zMult = 3.0f;
        minZ < 0 ? minZ *= zMult : minZ /= zMult;
        maxZ < 0 ? maxZ /= zMult : maxZ *= zMult;

        const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return lightProjection * lightView;
    }
} // Kita
