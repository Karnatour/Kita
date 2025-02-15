#include <iostream>
#include "KitaEngine/Kita.h"

class Sandbox : public IGameInstance {
public:
    void onInit() override {
        window.init();
        window.createWindow(500, 500, "Kita");
    }

    void onUpdate() override {

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
