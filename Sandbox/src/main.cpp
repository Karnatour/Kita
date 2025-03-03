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
        if (Kita::Input::isKeyPressed(Kita::KeyboardKey::KEY_K, {Kita::KeyModifiers::MODIFIER_CTRL,Kita::KeyModifiers::MODIFIER_SHIFT})) {
            KITA_DEBUG("Keypressed");
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
    Sandbox* gameInstance = new Sandbox();
    registerGameInstance(gameInstance);
}
