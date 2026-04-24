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
                    const glm::vec4 pt =
                        inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustumCorners.emplace_back(pt / pt.w);
                }
            }
        }

        return frustumCorners;
    }

    glm::mat4 LightUtil::getLightSpaceMatrix(const CameraProperties& properties, const float zNear, const float zFar, const std::pair<float, float> resolution) {
        const glm::mat4 projection = glm::perspective(glm::radians(properties.zoom), resolution.first / resolution.second, zNear, zFar);
        const std::vector<glm::vec4> corners = getFrustrumPoints(CameraUtil::getViewMatrix(properties), projection);

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const auto& corner : corners) {
            center += glm::vec3(corner);
        }
        center /= corners.size();

        const glm::mat4 lightView = glm::lookAt(center + lightDir, center, glm::vec3(0.0f, 1.0f, 0.0f));

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::lowest();
        for (const auto& corner : corners) {
            const glm::vec3 lightViewSpaceCorner = lightView * corner;
            minX = std::min(minX, lightViewSpaceCorner.x);
            maxX = std::max(maxX, lightViewSpaceCorner.x);
            minY = std::min(minY, lightViewSpaceCorner.y);
            maxY = std::max(maxY, lightViewSpaceCorner.y);
            minZ = std::min(minZ, lightViewSpaceCorner.z);
            maxZ = std::max(maxZ, lightViewSpaceCorner.z);
        }

        constexpr float zMult = 10.0f; //TODO Config
        if (minZ < 0) {
            minZ *= zMult;
        }
        else {
            minZ /= zMult;
        }
        if (maxZ < 0) {
            maxZ /= zMult;
        }
        else {
            maxZ *= zMult;
        }

        const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return lightProjection * lightView;
    }
} // Kita
