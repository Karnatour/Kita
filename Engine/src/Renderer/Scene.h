#pragma once
#include "Mesh.h"
#include <unordered_map>

#include "Camera.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Scene {
    public:
        Scene();
        void render() const;
        void addMesh(Mesh&& mesh);
        void addMesh(Mesh& mesh);
        Camera& getCamera();
    private:
        std::unordered_map<unsigned int, Mesh> m_meshes{};
        Camera m_camera;
    };
} // Kita
