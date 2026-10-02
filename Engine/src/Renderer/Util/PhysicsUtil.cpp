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

    glm::mat4 PhysicsUtil::JPHToGLMMat4(const JPH::Mat44& mat) {
        glm::mat4 out;
        for (int c = 0; c < 4; ++c) {
            JPH::Vec4 col = mat.GetColumn4(c);
            out[c] = glm::vec4(col.GetX(), col.GetY(), col.GetZ(), col.GetW());
        }
        return out;
    }
} // Kita
