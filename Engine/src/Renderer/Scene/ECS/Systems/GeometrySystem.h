#pragma once
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../Entity.h"
#include "../../../../Assets/AssetManager.h"
#include "../../../Buffers/UniformBuffer.h"

namespace Kita {
    struct MaterialComponent;

    class KITAENGINE_API GeometrySystem : public System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;
    private:
        // std140 layout
        struct alignas(16) PhongPropertiesUBOLayout {
            glm::vec4 ambient = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec4 diffuse = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec4 specular = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            glm::vec4 params = glm::vec4(
                0.0f, // shininess
                0.0f, // unused
                0.0f, // unused
                0.0f //  unused
            );
        };

        std::array<Texture*, 4> fetchTextures(const AssetManager& assetManager, const MaterialComponent& materialCmp, Scene& scene);

        std::unique_ptr<UniformBuffer> m_phongPropertiesUBO;
    };
} // Kita
