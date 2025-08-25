#include "Sandbox.h"
#include <magic_enum/magic_enum.hpp>
#include "../../Engine/src/Asset/KAsset.h"

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("[Test] Key pressed {}", magic_enum::enum_name(event.getKey()));
}

void Sandbox::onInit() {
    //auto entity = std::make_shared<Kita::Entity>(Kita::AssetImporter::importModel("backpack/backpack.obj"));
    //m_scene.addEntity(entity);
    //std::shared_ptr<Kita::Entity> entity2 = entity->clone();
    //entity2->getTransformation().translate({0.0f, 5.0f, 0.0f});
    //m_scene.addEntity(entity2);
    //Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    m_scene.update();
}

void Sandbox::onRender() {
    m_scene.render();
}

void Sandbox::onExit() {
}
