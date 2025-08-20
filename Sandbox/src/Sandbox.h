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
    std::vector<Kita::Vertex> m_vertices = Kita::Cube::vertices;
    std::vector<unsigned int> m_indices = Kita::Cube::indices;
    std::shared_ptr<Kita::Model> m_model;
    Kita::Scene m_scene;
};
