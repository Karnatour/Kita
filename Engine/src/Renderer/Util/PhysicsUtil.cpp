#include "../../kitapch.h"
#include "PhysicsUtil.h"

namespace Kita {
    JPH::Float3 PhysicsUtil::GLMToJPHFloat3(glm::vec3 vec) {
        return {vec.x, vec.y, vec.z};
    }

    JPH::RVec3 PhysicsUtil::GLMToJPHRVec3(glm::vec3 vec) {
        return {vec.x, vec.y, vec.z};
    }

    JPH::Vec3 PhysicsUtil::GLMToJPHVec3(const glm::vec3 vec) {
        return {vec.x, vec.y, vec.z};
    }

    JPH::Quat PhysicsUtil::GLMToJPHQuat(glm::quat quat) {
        return {quat.x, quat.y, quat.z, quat.w};
    }

    glm::vec4 PhysicsUtil::JPHToGLMVec4(const JPH::Vec4 vec) {
        return {vec.GetX(), vec.GetY(), vec.GetZ(), vec.GetW()};
    }

    glm::vec3 PhysicsUtil::JPHToGLMVec3(const JPH::RVec3& vec) {
        return {vec.GetX(), vec.GetY(), vec.GetZ()};

    }
} // Kita
