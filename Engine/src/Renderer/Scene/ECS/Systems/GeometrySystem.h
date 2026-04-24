#pragma once
#include "../../../../Core/DllTemplate.h"
#include "System.h"
#include "../Entity.h"
#include "../../../../Assets/AssetManager.h"

namespace Kita {
    class KITAENGINE_API GeometrySystem : System {
    public:
        int getOrder() override;
        void update(Scene& scene) override;
        void render(Scene& scene) override;
    private:
        std::array<Texture*, 3> fetchTextures(const AssetManager& assetManager, Entity entity);
    };
} // Kita
