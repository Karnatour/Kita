#include "Sandbox.h"
#include <magic_enum/magic_enum.hpp>

#include "../../Engine/src/Renderer/Util/TransformationUtil.h"

void onSomething(Kita::KeyPressed& event) {
    KITA_DEBUG("[Test] Key pressed {}", magic_enum::enum_name(event.getKey()));
}

void Sandbox::onInit() {
    m_scene = std::make_unique<Kita::Scene>();
    m_scene->addDefaultSystems();
    Kita::AssetImporter::importModel("main_sponza/NewSponza_Main_glTF_003.gltf", *m_scene).value();
    Kita::AssetImporter::importModel("pkg_a_curtains/NewSponza_Curtains_glTF.gltf", *m_scene).value();
    m_sphere = Kita::AssetImporter::importModel("sphere-gltf-example/scene.gltf", *m_scene).value();
    auto sphereBodyID = m_sphere.getComponent<Kita::PhysicsComponent>().bodyID;

    auto& sphereTransform = m_sphere.getComponent<Kita::TransformationComponent>();
    sphereTransform.localModel = Kita::TransformationUtil::translateWorld(sphereTransform.localModel, glm::vec3(0.0f, 100.0f, 0.0f));
    sphereTransform.worldModel = sphereTransform.localModel;

    sphereBodyID = Kita::Engine::getEngine()->getPhysicsManager().changeMotionType(m_sphere, sphereBodyID, JPH::EMotionType::Dynamic, Kita::PhysicsLayers::MOVING, JPH::EActivation::Activate);
    m_sphere.getComponent<Kita::PhysicsComponent>().bodyID = sphereBodyID;

    auto lightEntity = m_scene->createEntity();
    lightEntity.addComponent<Kita::LightComponent>(Kita::LightComponent{.properties = Kita::LightProperties{.direction = glm::vec3(-0.1f, -1.0f, 0.1f), .lightType = Kita::LightType::DIRECTIONAL}});
    lightEntity.addComponent<Kita::DirectionalShadowComponent>(Kita::DirectionalShadowComponent{.properties = Kita::DirectionalShadowProperties{.cascadeCount = 4}});
    lightEntity.addComponent<Kita::CastsShadows>();
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

Kita::Scene& Sandbox::getScene() {
    return *m_scene;
}

extern "C" SANDBOX_API Kita::IGameInstance* createGameInstance() {
    return new Sandbox();
}
