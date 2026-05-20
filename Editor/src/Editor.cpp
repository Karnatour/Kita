#include "Editor.h"

void Editor::onInit() {
    m_sandbox = std::make_shared<Sandbox>();
    m_sandbox->onInit();
}

void Editor::onUpdate() {
    m_sandbox->onUpdate();
}

void Editor::onRender() {
    m_sandbox->onRender();
}

void Editor::onExit() {
    m_sandbox->onExit();
}
