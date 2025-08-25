#pragma once

#include "KitaEngine/Kita.h"
#include <vector>
#include <memory>

class Sandbox final : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void onRender() override;
    void onExit() override;

private:
    Kita::Scene m_scene;
};
