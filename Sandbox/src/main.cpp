#include <iostream>
#include "KitaEngine/Kita.h"

class Sandbox : public IGameInstance {
public:
    void onUpdate() override {

    }

    void onRender() override {

    }
};

int main() {
    Sandbox* gameInstance = new Sandbox();
    registerGameInstance(gameInstance);
    Kita::Engine::run();

}
