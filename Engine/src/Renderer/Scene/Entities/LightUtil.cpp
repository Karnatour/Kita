#include "../../../kitapch.h"
#include "LightUtil.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../RendererAPI.h"
#include "../../../Core/Engine.h"
#include "../../Util/GeometryUtil.h"

namespace Kita {
    LightUtil::LightUtil(LightType lightType) {
        m_lightProperties.lightType = static_cast<int>(lightType);
        m_shadowProperties.depthMapFBO->createBuffer(ShadowProperties::resolution, {{BufferType::DEPTH, FrameBuffer::AttachmentType::TEXTURE}},false);
        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultShadowMapVertex.glsl", "DefaultEmptyFragment.glsl");

        m_model = std::make_shared<Model>();

        const auto depthMaterial = std::make_shared<Material>();
        depthMaterial->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultShadowMapVertex.glsl", "DefaultEmptyFragment.glsl"));
        m_model->addMaterial(depthMaterial);

        const auto material = std::make_shared<Material>();
        material->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultVertex.glsl", "DefaultFragment.glsl"));
        material->addTexture(m_shadowProperties.depthMapFBO->getDepthTexture());
        m_model->addMaterial(material);

        //TODO Create dummy triangle
        m_model->addMesh(std::make_shared<Mesh>(std::vector<VertexProperties>(), std::vector<unsigned int>()));
        m_model->getMeshes().front()->setMaterialIndex(1);
    }

    bool LightUtil::onRender(RendererAPI& rendererApi) {
        return false;
    }

    void LightUtil::beginShadowMapRender(RendererAPI& rendererApi) {
        prepareLightModel();
        rendererApi.setViewport(ShadowProperties::resolution.first, ShadowProperties::resolution.second, false);
        m_shadowProperties.depthMapFBO->bind();
        rendererApi.clearBit({ClearBit::DEPTH});
    }

    void LightUtil::endShadowMapRender(RendererAPI& rendererApi) {
        rendererApi.restoreViewport();
        m_shadowProperties.depthMapFBO->unbind();
    }

    void LightUtil::prepareLightModel() {
        m_model->getMaterials().front()->getShader()->bind();
        float nearPlane = 0.1f, farPlane = 25.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(-5.5f, 14.0f, -1.5f),
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
        m_lightProperties.lightSpaceMatrix = lightProjection * lightView;
        m_model->getMaterials().front()->getShader()->setUniformMat4("lightSpaceMatrix", m_lightProperties.lightSpaceMatrix);
    }

} // Kita
