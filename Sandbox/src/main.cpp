#include <iostream>

#include "KitaEngine/Kita.h"

void necoSeStalo(Kita::WindowClosed& event) {
    KITA_DEBUG("Ahojky jsem event co se provedl");
}

class Sandbox : public IGameInstance {
public:
    void onInit() override {
        window.init();
        window.createWindow(500, 500, "Kita");
        Kita::EventManager::listenToEvent<Kita::WindowClosed>(necoSeStalo);
    }

    void onUpdate() override {
        if (Kita::Input::isKeyPressed(Kita::KeyboardKey::KEY_K, {Kita::Modifiers::MODIFIER_CTRL,Kita::Modifiers::MODIFIER_SHIFT})) {
            KITA_DEBUG("Keypressed");
        }
        if (Kita::Input::isMousePressed(Kita::MouseButton::MBUTTON_LEFT,{Kita::Modifiers::MODIFIER_CTRL})) {
            KITA_DEBUG("MousePressed");
        }
    }

    void onRender() override {
        window.swapBuffers();
        window.poolEvents();
    }

private:
    Kita::Window window;
};

int main() {
    std::unique_ptr<Sandbox> gameInstance = std::make_unique<Sandbox>();
    registerGameInstance(gameInstance.get());
}
