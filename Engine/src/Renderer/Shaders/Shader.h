#pragma once
#include <string>
#include "../../Core/DllTemplate.h"
#include <memory>
#include <glm/glm.hpp>

namespace Kita {
    class KITAENGINE_API Shader {
    public:
        virtual ~Shader() = default;
        virtual void bind() = 0;
        unsigned int getProgram() const;
        static std::shared_ptr<Shader> createPtr();
        virtual void compileShader(const std::string& vertexPath, const std::string& fragmentPath) = 0;

        virtual void setUniformBool(const std::string& location, bool value) = 0;
        virtual void setUniformFloat(const std::string& location, float value) = 0;
        virtual void seUniformtInt(const std::string& location, int value) = 0;
        virtual void setVec2(const std::string& location, const glm::vec2& value) = 0;
        virtual void setVec3(const std::string& location, const glm::vec3& value) = 0;
        virtual void setVec4(const std::string& location, const glm::vec4& value) = 0;
        virtual void setMat2(const std::string& location, const glm::mat2& value) = 0;
        virtual void setMat3(const std::string& location, const glm::mat3& value) = 0;
        virtual void setMat4(const std::string& location, const glm::mat4& value) = 0;

        const std::pair<std::string, std::string>& getPath() const;

    protected:
        unsigned int m_program = 0;
        std::pair<std::string, std::string> m_path;
    };
} // Kita
