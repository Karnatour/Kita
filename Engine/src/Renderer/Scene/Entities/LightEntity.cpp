#include "../../../kitapch.h"
#include "LightEntity.h"

namespace Kita {
    LightEntity::LightEntity(LightType lightType) {
        m_lightProperties.lightType = static_cast<int>(lightType);
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
