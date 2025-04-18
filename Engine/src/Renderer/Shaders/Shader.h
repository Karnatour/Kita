#pragma once
#include <string>
#include "../../Core/DllTemplate.h"
#include <memory>

namespace Kita {
    class KITAENGINE_API Shader {
    public:
        virtual ~Shader() = default;
        virtual void bind() = 0;
        virtual void setUniformBool(const std::string& location, bool value) = 0;
        virtual void setUniformFloat(const std::string& location, float value) = 0;
        virtual void seUniformtInt(const std::string& location, int value) = 0;
        unsigned int getProgram() const;
        static std::shared_ptr<Shader> createPtr();
        virtual void compileShader(const std::string& vertexPath, const std::string& fragmentPath) = 0;

    protected:
        unsigned int m_program = 0;
    };
} // Kita
