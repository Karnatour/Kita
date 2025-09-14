#include "../../../kitapch.h"
#include "SkyboxEntity.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../../Core/Engine.h"
#include "../../Structs/GeometryHelper.h"

namespace Kita {
    SkyboxEntity::SkyboxEntity(const std::filesystem::path& texturePath) {
        m_skyboxTexture->createSkyboxTexture2D(texturePath);
        m_cubemapTexture->createCubemapTexture(std::make_pair(m_skyboxTexture->getWidth(), m_skyboxTexture->getHeight()));

        const auto resolution = Engine::getEngine()->getWindow().getResolution();

        m_frameBuffer->createBuffer(resolution, {{BufferType::COLOR, FrameBuffer::AttachmentType::TEXTURE}});

        setupCubemapViews();

        prepareSkyboxModel();
    }

    bool SkyboxEntity::onRender(RendererAPI& rendererApi) {
        if (!m_firstFrame) {
            return false;
        }

        rendererApi.setDepthFunc(DepthFunctions::LEQUAL);
        rendererApi.disableBufferWrite(BufferType::DEPTH);

        rendererApi.setViewport(m_skyboxTexture->getWidth(), m_skyboxTexture->getHeight(), false);
        renderToFramebuffer(rendererApi);
        rendererApi.restoreViewport();

        swapToFinalSkyboxMaterial();

        rendererApi.enableBufferWrite(BufferType::DEPTH);
        rendererApi.setDepthFunc(DepthFunctions::LESS);
        return true;
    }

    void SkyboxEntity::setupCubemapViews() {
        m_captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)));
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)));
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)));
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)));
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)));
        m_captureViews.push_back(glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0)));
    }

    void SkyboxEntity::prepareSkyboxModel() {
        m_model = std::make_shared<Model>();

        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultSkyboxBuilderVertex.glsl", "DefaultSkyboxBuilderFragment.glsl");

        const auto material = std::make_shared<Material>();
        material->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultSkyboxBuilderVertex.glsl", "DefaultSkyboxBuilderFragment.glsl"));
        material->addTexture(m_skyboxTexture);

        m_model->addMaterial(material);

        m_model->addMesh(std::make_shared<Mesh>(Cube::vertices, Cube::indices));

        m_model->getMeshes().front()->setMaterialIndex(0);
    }

    void SkyboxEntity::renderToFramebuffer(RendererAPI& rendererApi) {
        m_frameBuffer->bind();

        const auto shader = m_model->getMaterials().front()->getShader();
        shader->bind();

        for (int i = 0; i < 6; ++i) {
            m_frameBuffer->attachCubemapFace(m_cubemapTexture->getTexture(), i);

            shader->setUniformMat4("projection", m_captureProjection);
            shader->setUniformMat4("view", m_captureViews[i]);

            m_model->getMaterials().front()->getTextures().front()->bind(0);

            for (const auto& mesh : m_model->getMeshes()) {
                rendererApi.renderMesh(mesh);
            }
        }
        m_frameBuffer->unbind();
        m_renderOnce = true;
    }

    void SkyboxEntity::swapToFinalSkyboxMaterial() {
        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultSkyboxRenderVertex.glsl", "DefaultSkyboxRenderFragment.glsl");
        m_model->getMaterials().front()->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultSkyboxRenderVertex.glsl", "DefaultSkyboxRenderFragment.glsl"));
        m_model->getMaterials().front()->replaceTexture(m_cubemapTexture, 0);
    }
} // Kita
