#include "../kitapch.h"

#include "KAsset.h"
#include "../Core/Engine.h"
#include "../Renderer/Textures/TextureManager.h"

namespace Kita {
    std::shared_ptr<Model> KAsset::loadFromFile(const std::filesystem::path& path) {
        try {
            std::ifstream file(path, std::ios::binary | std::ios::in);
            file.exceptions(std::istream::failbit | std::istream::badbit);

            FileHeader header;
            file.read(reinterpret_cast<char*>(&header), sizeof(header));

            cheackHeaderFormat(header, path);

            auto model = std::make_shared<Model>();

            for (uint32_t i = 0; i < header.chunkCount; ++i) {
                ChunkHeader chunk;
                file.read(reinterpret_cast<char*>(&chunk), sizeof(chunk));
                loadChunk(file, chunk, model);
            }

            return model;
        }
        catch (const std::ifstream::failure& e) {
            KITA_ENGINE_ERROR("Error while loading aseet returning nullptr. Path {}, error: {}", path.string(), e.what());
            return nullptr;
        }
        catch (const std::runtime_error& e) {
            KITA_ENGINE_ERROR("Eror while loeading asset returning nullptr. Patr {}, error: {}", path.string(), e.what());
            return nullptr;
        }
    }

    bool KAsset::saveToFile(const std::shared_ptr<Model>& model, const std::filesystem::path& path) {
        try {
            std::filesystem::create_directories(path.parent_path());

            std::ofstream file(path, std::ios::binary | std::ios::trunc);
            file.exceptions(std::ofstream::failbit | std::ofstream::badbit);

            FileHeader header;
            writeHeaderStart(file, header, model->getMeshes().size() + model->getMaterials().size());

            writeMeshes(file, model->getMeshes());

            writeMaterials(file, model->getMaterials());

            writeHeaderEnd(file, header);

            m_bakedFiles.insert(path.lexically_relative("../assets/baked"));

            return true;
        }
        catch (const std::ofstream::failure& e) {
            KITA_ENGINE_ERROR("Error while saving aseet. Path {}, error: {}", path.string(), e.what());
            return false;
        }
        catch (const std::runtime_error& e) {
            KITA_ENGINE_ERROR("Eror while loeading asset returning nullptr. Patr {}, error: {}", path.string(), e.what());
            return false;
        }
    }

    void KAsset::fetchExistingBakedFiles() {
        const std::filesystem::path bakedPath = "../assets/baked/";

        if (!std::filesystem::exists(bakedPath)) {
            return;
        }

        for (auto& entry : std::filesystem::recursive_directory_iterator(bakedPath)) {
            if (entry.is_regular_file()) {
                std::filesystem::path relativePath = std::filesystem::relative(entry.path(), bakedPath);
                m_bakedFiles.insert(relativePath.string());
            }
        }
    }

    bool KAsset::alreadyBaked(const std::filesystem::path& path) {
        return m_bakedFiles.contains(path);
    }

    void KAsset::loadChunk(std::ifstream& file, ChunkHeader& chunk, const std::shared_ptr<Model>& model) {
        const auto chunkStart = file.tellg();

        switch (static_cast<ChunkType>(chunk.chunkType)) {
            case ChunkType::MESH: {
                model->addMesh(readMesh(file));
                break;
            }
            case ChunkType::MATERIAL: {
                model->addMaterial(readMaterial(file));
                break;
            }
            case ChunkType::NONE: {
                break;
            }
        }

        if (auto chunkEnd = file.tellg(); static_cast<uint64_t>(chunkEnd - chunkStart) != chunk.chunkSize) {
            throw std::runtime_error("Chunk size mismatch! Expected " + std::to_string(chunk.chunkSize) + ", read " + std::to_string(chunkEnd - chunkStart));
        }
    }

    void KAsset::cheackHeaderFormat(const FileHeader& header, const std::filesystem::path& path) {
        if (header.magicNumber != MAGIC_NUMBER) {
            throw std::runtime_error("Invalid magic number in asset file: " + path.string());
        }
        if (header.versionFormat != VERSION_FORMAT) {
            throw std::runtime_error("Unsupported version in asset file: " + path.string());
        }
    }

    std::shared_ptr<Mesh> KAsset::readMesh(std::ifstream& file) {
        MeshHeader meshHeader;
        file.read(reinterpret_cast<char*>(&meshHeader), sizeof(meshHeader));
        std::vector<Vertex> vertices(meshHeader.vertexCount);
        file.read(reinterpret_cast<char*>(vertices.data()), static_cast<std::streamsize>(vertices.size() * sizeof(Vertex)));

        std::vector<unsigned int> indices(meshHeader.indexCount);
        file.read(reinterpret_cast<char*>(indices.data()), static_cast<std::streamsize>(indices.size() * sizeof(unsigned int)));

        return std::make_shared<Mesh>(vertices, indices, meshHeader.materialIndex);
    }

    std::shared_ptr<Material> KAsset::readMaterial(std::ifstream& file) {
        MaterialHeader materialHeader{};
        file.read(reinterpret_cast<char*>(&materialHeader), sizeof(materialHeader));

        auto material = std::make_shared<Material>();
        for (int j = 0; j < materialHeader.textureCount; ++j) {
            auto& textureManager = Engine::getEngine()->getRenderer().getTextureManager();
            textureManager.addTexture(materialHeader.texturePaths[j]);
            material->addTexture(textureManager.getTexture(materialHeader.texturePaths[j]));
        }

        auto& [ambient, diffuse, specular, shininess] = material->getPhongProperties();
        FloatToVec3(ambient, materialHeader.ambient);
        FloatToVec3(diffuse, materialHeader.diffuse);
        FloatToVec3(specular, materialHeader.specular);
        shininess = materialHeader.shininess;

        auto& shaderManager = Engine::getEngine()->getRenderer().getShaderManager();
        shaderManager.addShader(materialHeader.shaderPaths[0], materialHeader.shaderPaths[1]);
        material->setShader(shaderManager.getShader(materialHeader.shaderPaths[0], materialHeader.shaderPaths[1]));

        return material;
    }

    void KAsset::writeHeaderStart(std::ofstream& file, FileHeader& header, const uint32_t chunkCount) {
        header.chunkCount = chunkCount;
        file.write(reinterpret_cast<const char*>(&header), sizeof(header));
    }

    void KAsset::writeHeaderEnd(std::ofstream& file, const FileHeader& header) {
        const auto endPos = file.tellp();
        const uint64_t totalSize = endPos;

        file.seekp(offsetof(FileHeader, chunkCount));
        file.write(reinterpret_cast<const char*>(&header.chunkCount), sizeof(header.chunkCount));

        file.seekp(offsetof(FileHeader, totalSize));
        file.write(reinterpret_cast<const char*>(&totalSize), sizeof(totalSize));
    }

    void KAsset::writeMeshes(std::ofstream& file, const std::vector<std::shared_ptr<Mesh>>& meshes) {
        for (const auto& mesh : meshes) {
            const auto vbo = mesh->getVertexArray()->getVBOobj();
            const auto ibo = mesh->getVertexArray()->getIBOobj();

            const auto& vertices = vbo->getVertices();
            const auto& indices = ibo->getIndices();

            MeshHeader meshHeader;
            meshHeader.vertexCount = static_cast<uint32_t>(vertices.size());
            meshHeader.indexCount = static_cast<uint32_t>(indices.size());
            meshHeader.materialIndex = mesh->getMaterialIndex();

            ChunkHeader chunk;
            chunk.chunkType = static_cast<uint32_t>(ChunkType::MESH);
            chunk.chunkSize = sizeof(MeshHeader) + sizeof(Vertex) * vertices.size() + sizeof(unsigned int) * indices.size();
            file.write(reinterpret_cast<const char*>(&chunk), sizeof(chunk));

            file.write(reinterpret_cast<const char*>(&meshHeader), sizeof(meshHeader));
            file.write(reinterpret_cast<const char*>(vertices.data()), static_cast<std::streamsize>(vertices.size() * sizeof(Vertex)));
            file.write(reinterpret_cast<const char*>(indices.data()), static_cast<std::streamsize>(indices.size() * sizeof(unsigned int)));
        }
    }

    void KAsset::writeMaterials(std::ofstream& file, const std::vector<std::shared_ptr<Material>>& materials) {
        for (const auto& material : materials) {
            MaterialHeader materialHeader{};
            materialHeader.textureCount = material->getTextures().size();

            auto [ambient, diffuse, specular, shininess] = material->getPhongProperties();
            vec3ToFloat(ambient, materialHeader.ambient);
            vec3ToFloat(diffuse, materialHeader.diffuse);
            vec3ToFloat(specular, materialHeader.specular);
            materialHeader.shininess = shininess;

            writeTextures(materialHeader, material->getTextures());

            writeShader(materialHeader, material->getShader());

            ChunkHeader chunk;
            chunk.chunkType = static_cast<uint32_t>(ChunkType::MATERIAL);
            chunk.chunkSize = sizeof(MaterialHeader);
            file.write(reinterpret_cast<const char*>(&chunk), sizeof(chunk));

            file.write(reinterpret_cast<const char*>(&materialHeader), sizeof(materialHeader));
        }
    }

    void KAsset::writeTextures(MaterialHeader& materialHeader, const std::vector<std::shared_ptr<Texture>>& textures) {
        for (int i = 0; i < materialHeader.textureCount; i++) {
            if (textures.at(i)->getPath().string().length() >= 63) {
                throw std::runtime_error("TexturePath is larger than 64 causing overflow " + textures.at(i)->getPath().string());
            }
            strcpy_s(materialHeader.texturePaths[i], textures.at(i)->getPath().string().c_str());
        }
    }

    void KAsset::writeShader(MaterialHeader& materialHeader, const std::shared_ptr<Shader>& shader) {
        //Vertex
        auto [vertexPath, fragmentPath] = shader->getPath();
        if (vertexPath.string().length() >= 63) {
            throw std::runtime_error("VertexShaderPath is larger than 64 causing overflow " + vertexPath.string());
        }
        strcpy_s(materialHeader.shaderPaths[0], vertexPath.string().c_str());

        //Fragment
        if (fragmentPath.string().length() >= 63) {
            throw std::runtime_error("FragmentShaderPath is larger than 64 causing overflow " + fragmentPath.string());
        }
        strcpy_s(materialHeader.shaderPaths[1], fragmentPath.string().c_str());
    }

    void KAsset::vec3ToFloat(const glm::vec3& vec, float (&arr)[3]) {
        arr[0] = vec.x;
        arr[1] = vec.y;
        arr[2] = vec.z;
    }

    void KAsset::FloatToVec3(glm::vec3& vec, const float (&arr)[3]) {
        vec.x = arr[0];
        vec.y = arr[1];
        vec.z = arr[2];
    }
} // Kita
