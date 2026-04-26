#include "Sandbox.h"

#include <glm/ext/matrix_transform.hpp>
#include <magic_enum/magic_enum.hpp>
#include "../../Engine/src/Assets/KAsset.h"

std::shared_ptr<Kita::LightUtil> lightEntity;
std::shared_ptr<Kita::LightUtil> lightEntity2;

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("[Test] Key pressed {}", magic_enum::enum_name(event.getKey()));
}

void Sandbox::onInit() {
    m_scene = std::make_unique<Kita::Scene>();
    m_scene->addDefaultSystems();

    //Kita::Entity entity = Kita::AssetImporter::importModel("Sponza-master/sponza.obj", *m_scene, true);
    //m_scene.addEntity(entity);
    ///entity->getTransformation().scale({0.005f, 0.005f, 0.005f});
    //lightEntity2 = std::make_shared<Kita::LightUtil>(Kita::LightUtil::LightType::DIRECTIONAL);
    //lightEntity2->setDirection({-0.1f, -0.7f, -0.2f, 0.0f});
    //m_scene.addEntity(lightEntity);
    //m_scene.addEntity(lightEntity2);
    Kita::EventManager::listenToEvent<Kita::KeyPressed>(onSomething);
}

void Sandbox::onUpdate() {
    m_scene->update();
}

void Sandbox::onRender() {
    m_scene->render();
}

void Sandbox::onExit() {
}
