#pragma once
#include "Material.h"
#include "Mesh.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Model {
    public:
        Model();
        void addMesh(std::shared_ptr<Mesh> mesh);
        void addMaterial(std::shared_ptr<Material> material);

        std::vector<std::shared_ptr<Mesh>>& getMeshes();
        std::vector<std::shared_ptr<Material>>& getMaterials();

        unsigned int getID() const;
    private:
        inline static unsigned int m_idCounter = 0;
        unsigned int m_id;
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<Material>> m_materials;

    };
} // Kita
