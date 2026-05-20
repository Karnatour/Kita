#include "Editor.h"

int main() {
    auto engine = Kita::Engine::getEngine();
    engine->init(Kita::RenderingAPI::OPENGL);

    std::shared_ptr<Editor> editorInstance = std::make_shared<Editor>();
    engine->loadGameInstance(editorInstance);

    return 0;
}
