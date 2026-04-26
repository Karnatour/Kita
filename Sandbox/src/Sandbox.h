#pragma once
#include <memory>
#include "KitaEngine/Kita.h"

class Sandbox final : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void onRender() override;
    void onExit() override;

private:
    std::unique_ptr<Kita::Scene> m_scene;
};
