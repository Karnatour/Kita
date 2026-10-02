#include "Sandbox.h"
#include <magic_enum/magic_enum.hpp>

#include "../../Engine/src/Renderer/Scene/ECS/EntityBuilder.h"
#include "../../Engine/src/Renderer/Util/PlayerUtil.h"
#include "../../Engine/src/Renderer/Util/TransformationUtil.h"

void Sandbox::onInit() {
    m_scene = std::make_unique<Kita::Scene>();
    m_scene->addDefaultSystems();

    Kita::TransformationComponent scaledTransform;
    glm::mat4 groundMatrix = glm::mat4(1.0f);
    groundMatrix = Kita::TransformationUtil::scaleWorld(groundMatrix, glm::vec3(100.0f, 2.0f, 100.0f));
    groundMatrix = Kita::TransformationUtil::translateWorld(groundMatrix, glm::vec3(0.0f, -5.0f, 0.0f));
    scaledTransform.localModel = groundMatrix;
    Kita::Entity rootEntity = Kita::EntityBuilder::createRootRenderEntity(*m_scene, "Ground", scaledTransform);
    Kita::Entity cubeEntity = Kita::EntityBuilder::createChildRenderEntity(*m_scene, Kita::Engine::getEngine()->getAssetManager().createAsset<Kita::Mesh>(Kita::Geometry::getCubeData()), Kita::AssetManager::DEFAULT_ASSET_ID,
                                                                           Kita::AssetManager::DEFAULT_ASSET_ID);
    rootEntity.getComponent<Kita::ChildrenComponent>().children.emplace_back(cubeEntity);
    Kita::EntityBuilder::finalizeStaticBody(rootEntity, Kita::PhysicsLayers::STATIC, JPH::EActivation::Activate);


    //Kita::AssetImporter::importModel("main_sponza/NewSponza_Main_glTF_003.gltf", *m_scene).value();
    //Kita::AssetImporter::importModel("pkg_a_curtains/NewSponza_Curtains_glTF.gltf", *m_scene).value();
    m_sphere = Kita::AssetImporter::importModel("sphere-gltf-example/scene.gltf", *m_scene).value();
    auto sphereBodyID = m_sphere.getComponent<Kita::PhysicsComponent>().bodyID;
    sphereBodyID = Kita::Engine::getEngine()->getPhysicsManager().changeMotionType(m_sphere, sphereBodyID, JPH::EMotionType::Dynamic, Kita::PhysicsLayers::MOVING, JPH::EActivation::Activate);
    m_sphere.getComponent<Kita::PhysicsComponent>().bodyID = sphereBodyID;
    Kita::TransformationUtil::setWorldPosition(m_sphere, glm::vec3(5.0f, 10.0f, 5.0f));

    m_player = Kita::EntityBuilder::createPlayerCharacter(*m_scene, "Player", glm::vec3(1.0f, 5.0f, 0.0f));

    m_vehicle = Kita::EntityBuilder::createVehicle(*m_scene, "911final/911f.gltf", glm::vec3(0.0f, 10.0f, 0.0f));

    Kita::EntityBuilder::createDirectionalLight(*m_scene);

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
        if (!Kita::CameraUtil::isCameraActive(m_player)) {
            Kita::CameraUtil::markCameraAsActive(m_player);
        } else {
            auto sceneCamera = Kita::Entity(m_scene.get(), m_scene->getCameraEntity());
            sceneCamera.getComponent<Kita::CameraComponent>().properties.position = m_player.getComponent<Kita::PlayerCharacterComponent>().properties.position;
            sceneCamera.getComponent<Kita::CameraComponent>().properties.pitch = m_player.getComponent<Kita::CameraComponent>().properties.pitch;
            sceneCamera.getComponent<Kita::CameraComponent>().properties.yaw = m_player.getComponent<Kita::CameraComponent>().properties.yaw;
            sceneCamera.getComponent<Kita::CameraComponent>().properties.front = m_player.getComponent<Kita::CameraComponent>().properties.front;
            sceneCamera.getComponent<Kita::CameraComponent>().properties.right = m_player.getComponent<Kita::CameraComponent>().properties.right;
            sceneCamera.getComponent<Kita::CameraComponent>().properties.up = m_player.getComponent<Kita::CameraComponent>().properties.up;
            Kita::CameraUtil::markCameraAsActive(sceneCamera);
        }
    }

    if (Kita::Input::isKeyPressed(Kita::InputKeys::KeyboardKey::KEY_T)) {
        if (!Kita::CameraUtil::isCameraActive(m_vehicle)) {
            Kita::CameraUtil::markCameraAsActive(m_vehicle);
        } else {
            Kita::PlayerUtil::setPosition(m_player, Kita::TransformationUtil::getPosition(m_vehicle.getComponent<Kita::TransformationComponent>().worldModel) + glm::vec3(2.0f, 1.0f, 2.0f));
            Kita::CameraUtil::markCameraAsActive(m_player);
        }
    }

    if (Kita::Input::isKeyPressed(Kita::InputKeys::KeyboardKey::KEY_B)) {
        if (Kita::CameraUtil::isCameraActive(m_vehicle)) {
            if (m_vehicle.getComponent<Kita::VehicleComponent>().properties.parked) {
                m_vehicle.getComponent<Kita::VehicleComponent>().properties.parked = false;
            } else {
                m_vehicle.getComponent<Kita::VehicleComponent>().properties.parked = true;
            }
        }
    }
}

extern "C" SANDBOX_API Kita::IGameInstance* createGameInstance() {
    return new Sandbox();
}
