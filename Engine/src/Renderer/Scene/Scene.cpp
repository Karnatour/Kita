#include "../../kitapch.h"
#include "Scene.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "../../Core/Engine.h"

namespace Kita {
    Scene::Scene() : m_camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
        m_cameraUniformBuffer->createBuffer(sizeof(m_camera.getCameraData()), &m_camera.getCameraData());
        m_skyboxEntity = std::make_shared<SkyboxEntity>("defaultSkybox.hdr");
    }

    void Scene::render() const{
        for (const auto& entity : m_entities | std::views::values) {
            Engine::getEngine()->getRenderer().getRendererAPI().render(entity);
        }
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        Engine::getEngine()->getRenderer().getRendererAPI().render(m_skyboxEntity);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    void Scene::addEntity(const std::shared_ptr<Entity>& entity) {
        m_entities.emplace(entity->getID(), entity);

        if (const auto lightEntity = std::dynamic_pointer_cast<LightEntity>(entity)) {
            addLight(lightEntity->getLightProperties());
        }
    }

    Camera& Scene::getCamera() {
        return m_camera;
    }

    void Scene::update() {
        m_camera.update();
        updateCameraBuffer();
        updateLights();
    }

    std::unordered_map<unsigned int, std::shared_ptr<Entity>>& Scene::getEntities() {
        return m_entities;
    }

    void Scene::updateCameraBuffer() {
        CameraProperties& data = m_camera.getCameraData();

        auto [width, height] = Engine::getEngine()->getWindow().getResolution();
        data.projection = glm::perspective(glm::radians(m_camera.getZoom()), static_cast<float>(width) / static_cast<float>(height), 0.5f, 1000.0f);
        data.view = m_camera.getViewMatrix();
        data.position = glm::vec4(m_camera.getPosition(), 1.0f);
        data.front = glm::vec4(m_camera.getFront(), 1.0f);

        m_cameraUniformBuffer->bind(0);
        m_cameraUniformBuffer->update(sizeof(data), &data);
    }

    //Maybe move to LightEntity ????
    std::vector<std::byte> Scene::buildLightsBuffer() const {
        //16 because of std430 rules;

        const size_t totalSize = LightEntity::LIGHT_COUNT_SIZE + m_lights.lights.size() * sizeof(LightEntity::LightProperties);
        std::vector<std::byte> buffer(totalSize);
        std::memcpy(buffer.data(), &m_lights.lightCount, LightEntity::LIGHT_COUNT_SIZE);

        for (size_t i = 0; i < m_lights.lights.size(); ++i) {
            std::memcpy(buffer.data() + LightEntity::LIGHT_COUNT_SIZE + i * sizeof(LightEntity::LightProperties), m_lights.lights[i], sizeof(LightEntity::LightProperties));
        }
        return buffer;
    }

    void Scene::updateLights() const {
        const std::vector<std::byte> buffer = buildLightsBuffer();

        m_lightsShaderStorageBuffer->bind(2);
        m_lightsShaderStorageBuffer->update(buffer.size(), buffer.data());
    }

    void Scene::addLight(LightEntity::LightProperties& lightProperties) {
        m_lights.lights.push_back(&lightProperties);
        m_lights.lightCount = static_cast<int>(m_lights.lights.size());

        const std::vector<std::byte> buffer = buildLightsBuffer();

        if (m_lights.lights.size() == 1) {
            // buffer.size() * LightEntity::MAX_LIGHTS isn't ideal since we include INT lightCount in the size, but reallocating should happen if we don't have enough space
            m_lightsShaderStorageBuffer->createBuffer(buffer.size() * LightEntity::MAX_LIGHTS, buffer.data());
        }

        m_lightsShaderStorageBuffer->bind(2);
        m_lightsShaderStorageBuffer->update(buffer.size(), buffer.data());
    }
} // Kita
