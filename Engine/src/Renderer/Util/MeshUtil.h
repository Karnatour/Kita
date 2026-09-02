#pragma once

#include "../../Core/DllTemplate.h"

namespace Kita {
    class Entity;
    class Mesh;

    struct MeshInstance {
        const Mesh* mesh;
        glm::mat4 worldModel = glm::mat4(1.0f);
    };

    struct KITAENGINE_API MeshUtil {
        static std::vector<Mesh*> getAllMeshesFromEntity(Entity rootEntity);
        static std::vector<MeshInstance> getAllMeshInstancesFromEntity(Entity rootEntity);
        static std::vector<MeshInstance> getAllMeshInstancesFromEntityRecursive(Entity entity, const glm::mat4& mat);
    };
} // Kita
