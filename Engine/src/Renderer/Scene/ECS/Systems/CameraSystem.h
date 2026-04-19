#pragma once
#include "System.h"
#include "../../../../Core/DllTemplate.h"
#include "../../../Buffers/UniformBuffer.h"
#include "../../../Util/CameraUtil.h"

namespace Kita {
    class Scene;

    class KITAENGINE_API CameraSystem final : public System {
    public:
        void update(Scene& scene) override;
        void render(Scene& scene) override;

    private:
        // std140 layout
        struct alignas(16) CameraUBOLayout {
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec4 front = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec4 params = glm::vec4(
                0.0f, // zNear
                0.0f, // zFar
                0.0f, // unused
                0.0f //  unused
            );
        };

        void updateActiveCameraData(const CameraProperties& properties);
        void updatePosition(CameraProperties&);
        void updateOrientationVectors(CameraProperties& properties);
        void updateEulerAngles(CameraProperties& properties);
        void updateZoom(CameraProperties& properties);
        std::unique_ptr<UniformBuffer> m_cameraUBO;
        CameraUBOLayout m_activeCameraData;
    };
} // Kita
