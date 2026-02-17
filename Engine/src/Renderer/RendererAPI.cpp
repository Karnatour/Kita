#include "../kitapch.h"
#include "RendererAPI.h"

#include "../Core/Engine.h"
#include "../Events/EventManager.h"
#include "GraphicsAPI/OpenGL/GLRendererAPI.h"
#include "Scene/Entities/LightEntity.h"
#include "Scene/Entities/SkyboxEntity.h"
#include "Structs/GeometryHelper.h"

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
        m_mainFramebuffer->bind();
        clearBit(std::initializer_list{ClearBit::COLOR, ClearBit::DEPTH, ClearBit::STENCIL});

        for (const auto& entity : entities) {
            renderEntity(entity);
        }

        //TODO Handle special render in SkyboxEntity itself ?
        renderSkyboxEntity(skyboxEntity);
        m_mainFramebuffer->unbind();
    }

    void RendererAPI::init(Renderer& renderer) {
        setupDebug();
        m_mainFramebuffer->createBuffer(m_viewport, std::initializer_list<FrameBuffer::AttachmentSpec>{
                                            {BufferType::COLOR, FrameBuffer::AttachmentType::TEXTURE}, {BufferType::DEPTH_STENCIL, FrameBuffer::AttachmentType::RENDERBUFFER}
                                        }, true);
        prepareScreenQuadModel(renderer);
        EventManager::listenToEvent<FrameBufferResized>(onFrameBufferResize);
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

    void RendererAPI::onFrameBufferResize(const FrameBufferResized& event) {
        const auto [width, height] = event.getSize();
        Engine::getEngine()->getRenderer().getRendererAPI().setViewport(width, height, true);
        Engine::getEngine()->getRenderer().getRendererAPI().m_mainFramebuffer->resize(event.getSize());
    }

    std::pair<int, int> RendererAPI::getViewport() const {
        return m_viewport;
    }

    void RendererAPI::renderSkyboxEntity(const std::shared_ptr<SkyboxEntity>& skyboxEntity) {
        setDepthFunc(DepthFunction::LEQUAL);
        disableBufferWrite(BufferType::DEPTH);
        renderEntity(std::static_pointer_cast<Entity>(skyboxEntity));
        enableBufferWrite(BufferType::DEPTH);
        setDepthFunc(DepthFunction::LESS);
    }

    void RendererAPI::renderMainFrameBufferToScreen() {
        m_mainFramebuffer->unbind();
        disableCapability(Capability::DEPTH_TEST);
        clearBit(std::initializer_list{ClearBit::COLOR});

        for (const auto& mesh : m_screenQuadModel->getMeshes()) {
            setMaterial(mesh->getMaterialIndex(), m_screenQuadModel->getMaterials());
            renderMesh(mesh);
        }

        enableCapability(Capability::DEPTH_TEST);
    }

    void RendererAPI::prepareScreenQuadModel(Renderer& renderer) {
        m_screenQuadModel = std::make_unique<Model>();

        renderer.getShaderManager().addShader("DefaultFrameBufferVertex.glsl", "DefaultFrameBufferFragment.glsl");

        const auto material = std::make_shared<Material>(renderer.getShaderManager().getShader("DefaultFrameBufferVertex.glsl", "DefaultFrameBufferFragment.glsl"));
        material->addTexture(m_mainFramebuffer->getColorTexture());

        m_screenQuadModel->addMaterial(material);

        m_screenQuadModel->addMesh(std::make_shared<Mesh>(Quad::vertices, Quad::indices));

        m_screenQuadModel->getMeshes().front()->setMaterialIndex(0);
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
                texture->bind(2);
                shader->setUniformInt("cubemapTex", 2);
                shader->setUniformBool("hasCubemapTex", true);
                break;
            case Texture::TextureType::COLOR:
                texture->bind(3);
                shader->setUniformInt("colorTex", 3);
                shader->setUniformBool("hasColorTex", true);
                break;
            case Texture::TextureType::DEPTH:
                texture->bind(4);
                shader->setUniformInt("depthTex", 4);
                shader->setUniformBool("hasDepthTex", true);
                break;
            case Texture::TextureType::STENCIL:
                texture->bind(5);
                shader->setUniformInt("stencilTex", 5);
                shader->setUniformBool("hasStencilTex", true);
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
            material->getShader()->setUniformBool("hasCubemapTex", false);
            material->getShader()->setUniformBool("hasColorTex", false);
            material->getShader()->setUniformBool("hasDepthTex", false);
            material->getShader()->setUniformBool("hasStencilTex", false);

            for (const auto& texture : material->getTextures()) {
                enableTextureInShader(material->getShader(), texture);
            }

            material->getShader()->setUniformMat4("model", transformation.getModelMatrix());
            material->getPhongUniformBuffer()->bind(1);
        }
    }

    void RendererAPI::setMaterial(const int materialIndex, const std::vector<std::shared_ptr<Material>>& materials) {
        if (materialIndex >= 0 && materialIndex < materials.size()) {
            const auto& material = materials[materialIndex];

            material->getShader()->bind();
            material->getShader()->setUniformBool("hasDiffuseTex", false);
            material->getShader()->setUniformBool("hasSpecularTex", false);
            material->getShader()->setUniformBool("hasCubemapTex", false);
            material->getShader()->setUniformBool("hasColorTex", false);
            material->getShader()->setUniformBool("hasDepthTex", false);
            material->getShader()->setUniformBool("hasStencilTex", false);

            for (const auto& texture : material->getTextures()) {
                enableTextureInShader(material->getShader(), texture);
            }

            material->getPhongUniformBuffer()->bind(1);
        }
    }

} // Kita
