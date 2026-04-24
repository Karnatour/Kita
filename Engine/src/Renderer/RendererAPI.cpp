#include "../kitapch.h"
#include "RendererAPI.h"

#include "../Core/Engine.h"
#include "../Events/EventManager.h"
#include "GraphicsAPI/OpenGL/GLRendererAPI.h"

namespace Kita {
    std::unique_ptr<RendererAPI> RendererAPI::createPtr(const RenderingAPI api) {
        switch (api) {
            case RenderingAPI::OPENGL:
                return std::make_unique<GLRendererAPI>();
            default:
                KITA_ENGINE_ERROR("Trying to create RendererAPI while RenderingAPI is not selected, returning nullptr");
                return nullptr;
        }
    }

    void RendererAPI::init(const Renderer& renderer) {
        setupDebug();
        prepareScreenQuadModel(renderer);
    }

    void RendererAPI::renderMainPass(const std::vector<std::shared_ptr<Entity>>& entities, const std::shared_ptr<SkyboxEntity>& skyboxEntity) {
        m_mainFramebuffer->bind();
        clearBit({ClearBit::COLOR, ClearBit::DEPTH, ClearBit::STENCIL});

        for (const auto& entity : entities) {
            renderEntity(entity);
        }

        //TODO Handle special render in SkyboxEntity itself ?
        renderSkyboxEntity(skyboxEntity);
        m_mainFramebuffer->unbind();
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
        clearBit({ClearBit::COLOR});

        for (const auto& mesh : m_screenQuadModel->getMeshes()) {
            setMaterial(mesh->getMaterialIndex(), m_screenQuadModel->getMaterials());
            renderMesh(mesh);
        }

        enableCapability(Capability::DEPTH_TEST);
    }

    void RendererAPI::prepareScreenQuadModel(const Renderer& renderer) {
        m_screenQuadModel = std::make_unique<Model>();

        renderer.getShaderManager().addShader("DefaultFrameBufferVertex.glsl", "DefaultFrameBufferFragment.glsl");

        const auto material = std::make_shared<Material>(renderer.getShaderManager().getShader("DefaultFrameBufferVertex.glsl", "DefaultFrameBufferFragment.glsl"));
        material->addTexture(m_mainFramebuffer->getColorTexture());

        m_screenQuadModel->addMaterial(material);

        m_screenQuadModel->addMesh(std::make_shared<Mesh>(Quad::vertices, Quad::indices));

        m_screenQuadModel->getMeshes().front()->setMaterialIndex(0);
    }
} // Kita
