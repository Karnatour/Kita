#include "../kitapch.h"
#include "Model.h"

namespace Kita {
    Model::Model() {
        m_id = ++m_idCounter;
    }

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

    unsigned int Model::getID() const {
        return m_id;
    }
} // Kita