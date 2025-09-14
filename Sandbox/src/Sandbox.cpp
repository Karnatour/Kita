#include "Sandbox.h"

#include <glm/ext/matrix_transform.hpp>
#include <magic_enum/magic_enum.hpp>
#include "../../Engine/src/Asset/KAsset.h"

std::shared_ptr<Kita::LightEntity> lightEntity;
std::shared_ptr<Kita::LightEntity> lightEntity2;

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("[Test] Key pressed {}", magic_enum::enum_name(event.getKey()));
}

void Sandbox::onInit() {
    auto entity = std::make_shared<Kita::Entity>(Kita::AssetImporter::importModel("Sponza-master/sponza.obj", false));
    m_scene.addEntity(entity);
    entity->getTransformation().scale({0.005f,0.005f,0.005f});
    lightEntity2 = std::make_shared<Kita::LightEntity>(Kita::LightEntity::LightType::DIRECTIONAL);
    lightEntity2->setDirection({-0.1f, -0.7f, -0.2f, 0.0f});
    //m_scene.addEntity(lightEntity);
    m_scene.addEntity(lightEntity2);
    Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    m_scene.update();
    KITA_ENGINE_DEBUG("X: {} Y: {} Z: {}",m_scene.getCamera().getPosition().x,m_scene.getCamera().getPosition().y,m_scene.getCamera().getPosition().z);
}

void Sandbox::onRender() {
    m_scene.render();
}

void Sandbox::onExit() {
}
