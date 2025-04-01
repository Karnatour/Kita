#include "GLShader.h"

namespace Kita {
    void GLShader::bind() {
        glUseProgram(m_program);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_program);
    }

    void GLShader::compileShader(const std::string& vertexPath, const std::string& fragmentPath) {
        KITA_ENGINE_DEBUG("Starting compilation of shader, vertexPath: {} fragmentPath: {}",vertexPath.c_str(),fragmentPath.c_str());
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        compileGLShader(vertexShader, vertexPath);
        checkShaderCompilation(vertexShader, vertexPath);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        compileGLShader(fragmentShader, fragmentPath);
        checkShaderCompilation(fragmentShader, fragmentPath);

        compileGLProgram(vertexShader, fragmentShader);
        checkProgramLinkage(vertexPath, fragmentPath);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void GLShader::setUniformBool(const std::string& location, const bool value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), static_cast<GLint>(value));
    }

    void GLShader::setUniformFloat(const std::string& location, const float value) {
        glUniform1f(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::seUniformtInt(const std::string& location, const int value) {
        glUniform1i(glGetUniformLocation(m_program, location.c_str()), value);
    }

    void GLShader::compileGLShader(const GLuint& shader, const std::string& shaderPath) {
        const std::string& shaderSource = FileReader::readFile(shaderPath);
        const char* sourcePtr = shaderSource.data();

        glShaderSource(shader, 1, &sourcePtr, nullptr);
        glCompileShader(shader);

    }

    void GLShader::checkShaderCompilation(const GLuint& shader, const std::string& shaderPath) {
        GLint vertexCompiled;
        glGetShaderiv(shader,GL_COMPILE_STATUS, &vertexCompiled);
        if (vertexCompiled == GL_FALSE) {
            GLsizei errorLength;
            GLchar errorMessage[1024];
            glGetShaderInfoLog(shader, 1024, &errorLength, errorMessage);
            KITA_ENGINE_ERROR("Unable to compile shader located in: {}\nGLError message ({}): {}", shaderPath.c_str(), errorLength, errorMessage);
        }
    }

    void GLShader::compileGLProgram(const GLuint vertexShader, const GLuint fragmentShader) {
        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);
    }

    void GLShader::checkProgramLinkage(const std::string& vertexPath, const std::string& fragmentPath) {
        GLint programLinked;
        glGetProgramiv(m_program,GL_LINK_STATUS, &programLinked);
        if (programLinked == GL_FALSE) {
            GLsizei errorLength;
            GLchar errorMessage[1024];
            glGetProgramInfoLog(m_program, 1024, &errorLength, errorMessage);
            KITA_ENGINE_ERROR("Unable to link program vertexShader {}, fragmentShader {}\nGLError message ({}): {}", vertexPath.c_str(), fragmentPath.c_str(),
                              errorLength, errorMessage);
        }
    }
} // Kita
