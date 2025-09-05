#pragma once
#include "Primitives/Mesh.h"
#include <unordered_map>

#include "Camera.h"
#include "Entity.h"
#include "../../Core/DllTemplate.h"
#include "../Buffers/ShaderStorageBuffer.h"
#include "../Buffers/UniformBuffer.h"
#include "Entities/LightEntity.h"
#include "Entities/SkyboxEntity.h"

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
        std::vector<std::byte> buildLightsBuffer() const;
        void updateLights() const;
        void addLight(LightEntity::LightProperties& lightProperties);

        std::unordered_map<unsigned int, std::shared_ptr<Entity>> m_entities{};

        Camera m_camera;
        std::shared_ptr<UniformBuffer> m_cameraUniformBuffer = UniformBuffer::createPtr();

        LightEntity::Lights m_lights;
        std::shared_ptr<ShaderStorageBuffer> m_lightsShaderStorageBuffer = ShaderStorageBuffer::createPtr();

        std::shared_ptr<SkyboxEntity> m_skyboxEntity;
    };
} // Kita
