#pragma once
#include "KitaEngine/Kita.h"
#include "Sandbox.h"

class Editor final : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void onRender() override;
    void onExit() override;

private:
    std::shared_ptr<Sandbox> m_sandbox;
};
