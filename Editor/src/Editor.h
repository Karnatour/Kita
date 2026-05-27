#pragma once
#include <imgui.h>

#include "KitaEngine/Kita.h"
#include "Sandbox.h"

class Editor final : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void buildInitialLayout(ImGuiID dockspace_id);
    void onRender() override;
    void onExit() override;

private:
    std::shared_ptr<Sandbox> m_sandbox;
    bool m_layoutInitialized = false;
};
