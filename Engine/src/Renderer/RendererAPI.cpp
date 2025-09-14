#include "../kitapch.h"
#include "RendererAPI.h"

#include "GraphicsAPI/OpenGL/GLRendererAPI.h"
#include "Scene/Entities/LightEntity.h"
#include "Scene/Entities/SkyboxEntity.h"

namespace Kita {
    void RendererAPI::renderShadowPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::vector<std::shared_ptr<LightEntity>>& lightEntities) {
        for (auto& lightEntity : lightEntities) {
            lightEntity->beginShadowMapRender(*this);

            for (auto& entity : entities) {
                if (auto model = entity->getModel()) {
                    for (auto& mesh : model->getMeshes()) {
                        lightEntity->getModel()->getMaterials().front()->getShader()->setUniformMat4("model", entity->getTransformation().getModelMatrix());
                        renderMesh(mesh);
                    }
                }
            }

            lightEntity->endShadowMapRender(*this);
        }
    }

    void RendererAPI::renderMainPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<SkyboxEntity>& skyboxEntity) {
        for (const auto& entity : entities) {
            renderEntity(entity);
        }
        //TODO Handle special render in SkyboxEntity itself ?
        renderSkyboxEntity(skyboxEntity);
    }

    std::unique_ptr<RendererAPI> RendererAPI::createPtr(const RenderingAPI api) {
        switch (api) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLRendererAPI>();
            default:
                KITA_ENGINE_ERROR("Trying to create RendererAPI while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    std::pair<int, int> RendererAPI::getViewport() const {
        return m_viewport;
    }

    void RendererAPI::renderSkyboxEntity(const std::shared_ptr<SkyboxEntity>& skyboxEntity) {
        setDepthFunc(DepthFunctions::LEQUAL);
        disableBufferWrite(BufferType::DEPTH);
        renderEntity(std::static_pointer_cast<Entity>(skyboxEntity));
        enableBufferWrite(BufferType::DEPTH);
        setDepthFunc(DepthFunctions::LESS);
    }

    void RendererAPI::renderEntity(const std::shared_ptr<Entity>& entity) {
        if (entity->onRender(*this)) {
            entity->setFirstFrame(false);
            return;
        }

        if (entity->getModel() != nullptr) {
            const auto& model = entity->getModel();
            for (const auto& mesh : model->getMeshes()) {
                setMaterial(mesh->getMaterialIndex(), model->getMaterials(), entity->getTransformation());
                renderMesh(mesh);
            }
        }
        entity->setFirstFrame(false);
    }

    void RendererAPI::renderMesh(const std::shared_ptr<Mesh>& mesh) {
        mesh->getVertexArray()->bind();
        const auto& vertexArray = mesh->getVertexArray();

        const size_t indicesCount = vertexArray->getIBOobj()->getIndicesCount();
        const size_t verticesCount = vertexArray->getVBOobj()->getVerticesCount();

        indicesCount == 0 ? drawArrays(verticesCount) : drawElements(indicesCount);
    }

    void RendererAPI::enableTextureInShader(const std::shared_ptr<Shader>& shader, const std::shared_ptr<Texture>& texture) {
        switch (texture->getType()) {
            case Texture::TextureType::DIFFUSE:
                texture->bind(0);
                shader->setUniformInt("diffuseTex", 0);
                shader->setUniformBool("hasDiffuseTex", true);
                break;
            case Texture::TextureType::SPECULAR:
                texture->bind(1);
                shader->setUniformInt("specularTex", 1);
                shader->setUniformBool("hasSpecularTex", true);
                break;
            case Texture::TextureType::CUBEMAP:
                texture->bind(3);
                shader->setUniformInt("cubemapTex", 3);
                break;
            case Texture::TextureType::DEPTH:
                texture->bind(4);
                shader->setUniformInt("depthTex", 4);
                shader->setUniformBool("hasDepthTex", true);
                break;
            default:
                KITA_ENGINE_WARN("Texture type is NONE or Unknown, unable to bind into shader", texture->getPath().string());
                break;
        }
    }

    void RendererAPI::setMaterial(const int materialIndex, const std::vector<std::shared_ptr<Material>>& materials, const Transformation& transformation) {
        if (materialIndex >= 0 && materialIndex < materials.size()) {
            const auto& material = materials[materialIndex];

            material->getShader()->bind();
            material->getShader()->setUniformBool("hasDiffuseTex", false);
            material->getShader()->setUniformBool("hasSpecularTex", false);
            material->getShader()->setUniformBool("hasDepthTex", false);

            for (const auto& texture : material->getTextures()) {
                enableTextureInShader(material->getShader(), texture);
            }

            material->getShader()->setUniformMat4("model", transformation.getModelMatrix());
            material->getPhongUniformBuffer()->bind(1);
        }
    }
} // Kita
