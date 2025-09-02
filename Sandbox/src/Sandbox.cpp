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
    auto entity = std::make_shared<Kita::Entity>(Kita::AssetImporter::importModel("backpack/backpack.obj", true));
    m_scene.addEntity(entity);
    lightEntity = std::make_shared<Kita::LightEntity>(Kita::LightEntity::LightType::SPOT);
    lightEntity2 = std::make_shared<Kita::LightEntity>(Kita::LightEntity::LightType::DIRECTIONAL);
    m_scene.addEntity(lightEntity);
    m_scene.addEntity(lightEntity2);
    Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    lightEntity->setDirection(glm::vec4(m_scene.getCamera().getFront(),1.0f));
    lightEntity->setPosition(glm::vec4(m_scene.getCamera().getPosition(),1.0f));
    m_scene.update();
}

void Sandbox::onRender() {
    m_scene.render();
}

void Sandbox::onExit() {
}
