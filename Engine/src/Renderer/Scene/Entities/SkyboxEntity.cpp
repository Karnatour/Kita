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
        m_depthRenderBuffer->createBuffer(resolution, BufferType::DEPTH);
        m_stencilRenderBuffer->createBuffer(resolution, BufferType::STENCIL);

        m_frameBuffer->createBuffer(resolution, 0, m_depthRenderBuffer->getRBO(), m_stencilRenderBuffer->getRBO());

        setupCubemapViews();

        prepareSkyboxModel();
    }

    bool SkyboxEntity::onRender(RendererAPI& rendererApi) {
        if (!m_firstFrame) {
            return false;
        }

        rendererApi.setViewport(m_skyboxTexture->getWidth(), m_skyboxTexture->getHeight(), false);
        renderToFramebuffer();
        rendererApi.restoreViewport();

        swapToFinalSkyboxMaterial();

        return true;
    }

    void SkyboxEntity::setupCubemapViews() {
        m_captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        m_captureViews[0] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0));
        m_captureViews[1] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0));
        m_captureViews[2] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
        m_captureViews[3] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1));
        m_captureViews[4] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0));
        m_captureViews[5] = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0));
    }

    void SkyboxEntity::prepareSkyboxModel() {
        m_model = std::make_shared<Model>();

        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultSkyboxBuilderVertex.glsl", "DefaultSkyboxBuilderFragment.glsl");

        const auto material = std::make_shared<Material>();
        material->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultSkyboxBuilderVertex.glsl", "DefaultSkyboxBuilderFragment.glsl"));
        material->addTexture(m_skyboxTexture);

        m_model->addMaterial(material);

        m_model->addMesh(std::make_shared<Mesh>(Cube::vertices, Cube::indices));

        m_model->getMeshes().at(0)->setMaterialIndex(0);
    }

    void SkyboxEntity::renderToFramebuffer() {
        m_frameBuffer->bind();
        m_model->getMaterials().at(0)->getShader()->bind();
        for (int i = 0; i < 6; ++i) {
            m_frameBuffer->attachCubemapFace(m_cubemapTexture->getTexture(), i);

            m_model->getMaterials().at(0)->getShader()->setUniformMat4("projection", m_captureProjection);
            m_model->getMaterials().at(0)->getShader()->setUniformMat4("view", m_captureViews[i]);

            m_model->getMaterials().at(0)->getTextures().at(0)->bind(0);

            for (const auto& mesh : m_model->getMeshes()) {
                mesh->getVertexArray()->bind();
                const auto& vertexArray = mesh->getVertexArray();

                if (vertexArray->getIBOobj()->getIndicesCount() == 0) {
                    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexArray->getVBOobj()->getVerticesCount()));
                }
                else {
                    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertexArray->getIBOobj()->getIndicesCount()),GL_UNSIGNED_INT, nullptr);
                }
            }
        }
        m_frameBuffer->unbind();
        m_renderOnce = true;
    }

    void SkyboxEntity::swapToFinalSkyboxMaterial() {
        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultSkyboxRenderVertex.glsl", "DefaultSkyboxRenderFragment.glsl");
        m_model->getMaterials().at(0)->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultSkyboxRenderVertex.glsl", "DefaultSkyboxRenderFragment.glsl"));
        m_model->getMaterials().at(0)->replaceTexture(m_cubemapTexture, 0);
    }
} // Kita
