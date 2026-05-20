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
        std::array<Texture*, 5> fetchTextures(const AssetManager& assetManager, const MaterialComponent& materialCmp, Scene& scene);
    };
} // Kita
