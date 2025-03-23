#pragma once
#include <string>
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Shader {
    public:
        virtual ~Shader() = default;

        virtual void compileShader(const std::string& vertexPath, const std::string& fragmentPath) = 0;
        virtual void bind() = 0;
        virtual void setUniformBool(const std::string& location, bool value) = 0;
        virtual void setUniformFloat(const std::string& location, float value) = 0;
        virtual void seUniformtInt(const std::string& location, int value) = 0;
        unsigned int getProgramID() const;

    protected:
        unsigned int m_program = 0;
    };
} // Kita
