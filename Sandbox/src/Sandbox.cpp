#include "Sandbox.h"
#include <magic_enum/magic_enum.hpp>

#include "../../Engine/src/Renderer/Scene/ECS/EntityBuilder.h"
#include "../../Engine/src/Renderer/Util/TransformationUtil.h"

void Sandbox::onInit() {
    m_scene = std::make_unique<Kita::Scene>();
    m_scene->addDefaultSystems();
    Kita::AssetImporter::importModel("main_sponza/NewSponza_Main_glTF_003.gltf", *m_scene).value();
    //Kita::AssetImporter::importModel("pkg_a_curtains/NewSponza_Curtains_glTF.gltf", *m_scene).value();
    //m_sphere = Kita::AssetImporter::importModel("sphere-gltf-example/scene.gltf", *m_scene).value();
    //auto sphereBodyID = m_sphere.getComponent<Kita::PhysicsComponent>().bodyID;

    //auto& sphereTransform = m_sphere.getComponent<Kita::TransformationComponent>();
    //sphereTransform.localModel = Kita::TransformationUtil::translateWorld(sphereTransform.localModel, glm::vec3(0.0f, 100.0f, 0.0f));
    //sphereTransform.worldModel = sphereTransform.localModel;

    //sphereBodyID = Kita::Engine::getEngine()->getPhysicsManager().changeMotionType(m_sphere, sphereBodyID, JPH::EMotionType::Dynamic, Kita::PhysicsLayers::MOVING, JPH::EActivation::Activate);
    //m_sphere.getComponent<Kita::PhysicsComponent>().bodyID = sphereBodyID;

    m_player = Kita::EntityBuilder::createPlayerCharacter(*m_scene, "Player", glm::vec3(1.0f,10.0f,0.0f));
    auto lightEntity = Kita::EntityBuilder::createDirectionalLight(*m_scene);
    Kita::EventManager::listenToEvent<Kita::KeyPressed>([this](const Kita::KeyPressed& event) {
        onKeyPressed(event);
    });
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

void Sandbox::onKeyPressed(const Kita::KeyPressed& event) {
    KITA_DEBUG("[Test] Key pressed {}", magic_enum::enum_name(event.getKey()));
    if (Kita::Input::isKeyPressed(Kita::InputKeys::KeyboardKey::KEY_C)) {
        if (Kita::CameraUtil::isCameraActive(Kita::Entity(m_scene.get(), m_scene->getCameraEntity()))) {
            Kita::CameraUtil::markCameraAsActive(m_player);
        } else {
            auto sceneCamera = Kita::Entity(m_scene.get(), m_scene->getCameraEntity());
            sceneCamera.getComponent<Kita::CameraComponent>().properties.position = m_player.getComponent<Kita::PlayerCharacterComponent>().properties.position;
            Kita::CameraUtil::markCameraAsActive(sceneCamera);
        }
    }
}

extern "C" SANDBOX_API Kita::IGameInstance* createGameInstance() {
    return new Sandbox();
}
