#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <Jolt/Jolt.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../../Core/DllTemplate.h"

namespace Kita {
    struct KITAENGINE_API PhysicsUtil {
        static JPH::Float3 GLMToJPHFloat3(glm::vec3 vec);
        static JPH::RVec3 GLMToJPHRVec3(glm::vec3 vec);
        static JPH::Vec3 GLMToJPHVec3(glm::vec3 vec);
        static JPH::Quat GLMToJPHQuat(glm::quat quat);
        static glm::vec4 JPHToGLMVec4(JPH::Vec4 vec);
        static glm::vec3 JPHToGLMVec3(const JPH::RVec3& vec);
    };
} // Kita
