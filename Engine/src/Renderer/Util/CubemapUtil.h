#pragma once
#include "../../Core/DllTemplate.h"
#include <vector>
#include <glm/fwd.hpp>

namespace Kita {
    struct KITAENGINE_API CubemapUtil {
        static std::vector<glm::mat4> getCubemapCaptureViews();
        static glm::mat4 getCubemapCaptureProjection();
    };
} // Kita
