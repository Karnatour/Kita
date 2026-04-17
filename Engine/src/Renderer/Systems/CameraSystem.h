#pragma once
#include "../../Core/DllTemplate.h"
#include "../Util/CameraUtil.h"

namespace Kita {
    class Scene;

    class KITAENGINE_API CameraSystem {
    public:
        void update(Scene& scene);

    private:
        void updatePosition(CameraProperties&);
        void updateOrientationVectors(CameraProperties& properties);
        void updateEulerAngles(CameraProperties& properties);
        void updateZoom(CameraProperties& properties);
    };
} // Kita
