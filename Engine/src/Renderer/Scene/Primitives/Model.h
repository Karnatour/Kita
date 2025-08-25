#pragma once
#include "Material.h"
#include "Mesh.h"
#include "../../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Model {
    public:
        void addMesh(std::shared_ptr<Mesh> mesh);
        void addMaterial(std::shared_ptr<Material> material);

        std::vector<std::shared_ptr<Mesh>>& getMeshes();
        std::vector<std::shared_ptr<Material>>& getMaterials();

        std::filesystem::path getPath();
        void setPath(const std::filesystem::path& path);
    private:
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<std::shared_ptr<Material>> m_materials;
        std::filesystem::path m_path;

    };
} // Kita
