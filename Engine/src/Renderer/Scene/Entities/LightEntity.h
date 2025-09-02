#pragma once
#include "../Entity.h"
#include "../../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API LightEntity final : public Entity {
    public:
        struct LightProperties {
            glm::vec4 position = {0.0f, 0.0f, 3.0f, 1.0f};
            glm::vec4 direction = {0.0f, 0.0f, 3.0f, 1.0f};
            glm::vec4 attenuation = {1.0f, 0.09f, 0.032f, 1.0f};
            glm::vec4 ambient = {0.0f, 0.0f, 0.0f, 1.0f};
            glm::vec4 diffuse = {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 specular = {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 cutOff = {glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)), 1.0f, 1.0f};
            alignas(16) int lightType;
        };

        struct Lights {
            alignas(16) int lightCount = 0;
            std::vector<LightProperties*> lights;
        };

        enum class LightType {
            DIRECTIONAL = 0,
            POINT = 1,
            SPOT = 2
        };

        static constexpr std::size_t LIGHT_COUNT_SIZE = 16;
        static constexpr int MAX_LIGHTS = 64;

        explicit LightEntity(LightType lightType);

        void setLightProperties(const LightProperties& lightProperties);
        void setPosition(const glm::vec4& position);
        void setDirection(const glm::vec4& direction);
        void setAttenuation(const glm::vec4& attenuation);
        void setAmbient(const glm::vec4& ambient);
        void setDiffuse(const glm::vec4& diffuse);
        void setSpecular(const glm::vec4& specular);
        void setCutOff(const glm::vec4& cutOff);
        void setLightType(const LightType& lightType);

        LightProperties& getLightProperties();
        glm::vec4 getPosition() const;
        glm::vec4 getDirection() const;
        glm::vec4 getAttenuation() const;
        glm::vec4 getAmbient() const;
        glm::vec4 getDiffuse() const;
        glm::vec4 getSpecular() const;
        glm::vec4 getCutOff() const;
        LightType getLightType() const;

    private:
        LightProperties m_lightProperties;
    };
} // Kita
