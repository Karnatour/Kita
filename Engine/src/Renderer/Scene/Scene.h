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
        const std::vector<std::shared_ptr<Entity>>& getEntities();
        Entity* getEntityByID(unsigned int id) const ;

    private:
        void updateCameraBuffer();
        std::vector<std::byte> buildLightsBuffer() const;
        void updateLights() const;
        void addLight(LightEntity::LightProperties& lightProperties);

        //Iterating through map is slow so we use vector for rendering
        std::vector<std::shared_ptr<Entity>> m_entities;
        std::unordered_map<unsigned int, Entity*> m_entityLookup;

        //Store special types of entities so we don't have to search them at runtime every frame
        std::vector<std::shared_ptr<LightEntity>> m_lightEntities;

        Camera m_camera;
        std::shared_ptr<UniformBuffer> m_cameraUniformBuffer = UniformBuffer::createPtr();

        LightEntity::Lights m_lights;
        std::shared_ptr<ShaderStorageBuffer> m_lightsShaderStorageBuffer = ShaderStorageBuffer::createPtr();

        std::shared_ptr<SkyboxEntity> m_skyboxEntity;
    };
} // Kita
