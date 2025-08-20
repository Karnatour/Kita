#pragma once
#include "Mesh.h"
#include <unordered_map>

#include "Camera.h"
#include "Model.h"
#include "../Core/DllTemplate.h"
#include "Buffers/UniformBuffer.h"

namespace Kita {
    class KITAENGINE_API Scene {
    public:
        Scene();
        void render() const;
        void addModel(Model model);
        Camera& getCamera();
        void update();
    private:
        void updateCameraBuffer();
        std::unordered_map<unsigned int, Model> m_models{};
        Camera m_camera;
        std::shared_ptr<UniformBuffer> m_cameraUniformBuffer = UniformBuffer::createPtr();
    };
} // Kita
