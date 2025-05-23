#pragma once
#include "Mesh.h"
#include <unordered_map>

#include "Camera.h"
#include "../Core/DllTemplate.h"
#include "Buffers/UniformBuffer.h"

namespace Kita {
    class KITAENGINE_API Scene {
    public:
        Scene();
        void render() const;
        void addMesh(Mesh&& mesh);
        void addMesh(Mesh& mesh);
        Camera& getCamera();
        void update();
    private:
        void updateCameraBuffer();
        std::unordered_map<unsigned int, Mesh> m_meshes{};
        Camera m_camera;
        std::shared_ptr<UniformBuffer> m_uniformBuffer = UniformBuffer::createPtr();

    };
} // Kita
