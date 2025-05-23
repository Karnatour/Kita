#pragma once

#include "KitaEngine/Kita.h"
#include <vector>
#include <memory>

class Sandbox : public Kita::IGameInstance {
public:
    void onInit() override;
    void onUpdate() override;
    void onRender() override;
    void onExit() override;

private:
    std::vector<Kita::Vertex> m_vertices = Kita::Cube::vertices;
    std::vector<unsigned int> m_indices = Kita::Cube::indices;
    std::shared_ptr<Kita::Mesh> m_mesh;
    Kita::Scene m_scene;
};
