#pragma once
#include "Primitives/Mesh.h"
#include <unordered_map>

#include "Camera.h"
#include "Entity.h"
#include "../../Core/DllTemplate.h"
#include "../Buffers/UniformBuffer.h"

namespace Kita {
    class KITAENGINE_API Scene {
    public:
        Scene();
        void render() const;
        void addEntity(const std::shared_ptr<Entity>& entity);
        Camera& getCamera();
        void update();
        std::unordered_map<unsigned int, std::shared_ptr<Entity>>& getEntities();
    private:
        void updateCameraBuffer();
        std::unordered_map<unsigned int, std::shared_ptr<Entity>> m_entities{};
        Camera m_camera;
        std::shared_ptr<UniformBuffer> m_cameraUniformBuffer = UniformBuffer::createPtr();
    };
} // Kita
