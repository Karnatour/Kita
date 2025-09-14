#include "../../../kitapch.h"
#include "LightEntity.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "../../RendererAPI.h"
#include "../../../Core/Engine.h"
#include "../../Structs/GeometryHelper.h"

namespace Kita {
    LightEntity::LightEntity(LightType lightType) {
        m_lightProperties.lightType = static_cast<int>(lightType);
        m_shadowProperties.depthMapFBO->createBuffer(ShadowProperties::resolution, {{BufferType::DEPTH, FrameBuffer::AttachmentType::TEXTURE}});
        Engine::getEngine()->getRenderer().getShaderManager().addShader("DefaultShadowMapVertex.glsl", "DefaultEmptyFragment.glsl");

        m_model = std::make_shared<Model>();

        const auto depthMaterial = std::make_shared<Material>();
        depthMaterial->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultShadowMapVertex.glsl", "DefaultEmptyFragment.glsl"));
        depthMaterial->addTexture(m_shadowProperties.depthMapFBO->getDepthTexture());
        m_model->addMaterial(depthMaterial);

        const auto material = std::make_shared<Material>();
        material->setShader(Engine::getEngine()->getRenderer().getShaderManager().getShader("DefaultVertex.glsl", "DefaultFragment.glsl"));
        material->addTexture(m_shadowProperties.depthMapFBO->getDepthTexture());
        m_model->addMaterial(material);

        m_model->addMesh(std::make_shared<Mesh>(std::vector<Vertex>(), std::vector<unsigned int>()));
        m_model->getMeshes().front()->setMaterialIndex(1);
    }

    bool LightEntity::onRender(RendererAPI& rendererApi) {
        return false;
    }

    void LightEntity::beginShadowMapRender(RendererAPI& rendererApi) {
        prepareLightModel();
        rendererApi.setViewport(ShadowProperties::resolution.first, ShadowProperties::resolution.second, false);
        m_shadowProperties.depthMapFBO->bind();
        rendererApi.clearBit({ClearBit::DEPTH});
    }

    void LightEntity::endShadowMapRender(RendererAPI& rendererApi) {
        rendererApi.restoreViewport();
        m_shadowProperties.depthMapFBO->unbind();
    }

    void LightEntity::prepareLightModel() {
        m_model->getMaterials().front()->getShader()->bind();
        float nearPlane = 0.1f, farPlane = 25.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
        glm::mat4 lightView = glm::lookAt(glm::vec3(-5.5f, 14.0f, -1.5f),
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));
        m_lightProperties.lightSpaceMatrix = lightProjection * lightView;
        m_model->getMaterials().front()->getShader()->setUniformMat4("lightSpaceMatrix", m_lightProperties.lightSpaceMatrix);
    }


    void LightEntity::setLightProperties(const LightProperties& lightProperties) {
        m_lightProperties = lightProperties;
    }

    void LightEntity::setPosition(const glm::vec4& position) {
        m_lightProperties.position = position;
    }

    void LightEntity::setDirection(const glm::vec4& direction) {
        m_lightProperties.direction = direction;
    }

    void LightEntity::setAttenuation(const glm::vec4& attenuation) {
        m_lightProperties.attenuation = attenuation;
    }

    void LightEntity::setAmbient(const glm::vec4& ambient) {
        m_lightProperties.ambient = ambient;
    }

    void LightEntity::setDiffuse(const glm::vec4& diffuse) {
        m_lightProperties.diffuse = diffuse;
    }

    void LightEntity::setSpecular(const glm::vec4& specular) {
        m_lightProperties.specular = specular;
    }

    void LightEntity::setCutOff(const glm::vec4& cutOff) {
        m_lightProperties.cutOff = cutOff;
    }

    void LightEntity::setLightType(const LightType& lightType) {
        m_lightProperties.lightType = static_cast<int>(lightType);
    }

    glm::vec4 LightEntity::getCutOff() const {
        return m_lightProperties.cutOff;
    }

    LightEntity::LightType LightEntity::getLightType() const {
        return static_cast<LightType>(m_lightProperties.lightType);
    }

    LightEntity::LightProperties& LightEntity::getLightProperties() {
        return m_lightProperties;
    }

    glm::vec4 LightEntity::getPosition() const {
        return m_lightProperties.position;
    }

    glm::vec4 LightEntity::getDirection() const {
        return m_lightProperties.direction;
    }

    glm::vec4 LightEntity::getAttenuation() const {
        return m_lightProperties.attenuation;
    }

    glm::vec4 LightEntity::getAmbient() const {
        return m_lightProperties.ambient;
    }

    glm::vec4 LightEntity::getDiffuse() const {
        return m_lightProperties.diffuse;
    }

    glm::vec4 LightEntity::getSpecular() const {
        return m_lightProperties.specular;
    }
} // Kita
