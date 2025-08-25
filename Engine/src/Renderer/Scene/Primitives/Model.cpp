#include "Model.h"
#include "../../../kitapch.h"

namespace Kita {
    void Model::addMesh(std::shared_ptr<Mesh> mesh) {
        m_meshes.push_back(std::move(mesh));
    }

    void Model::addMaterial(std::shared_ptr<Material> material) {
        m_materials.push_back(std::move(material));
    }

    std::vector<std::shared_ptr<Mesh>>& Model::getMeshes() {
        return m_meshes;
    }

    std::vector<std::shared_ptr<Material>>& Model::getMaterials() {
        return m_materials;
    }

    std::filesystem::path Model::getPath() {
        return m_path;
    }

    void Model::setPath(const std::filesystem::path& path) {
        m_path = path;
    }
} // Kita