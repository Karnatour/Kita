#include "Window.h"

#include "Logger.h"

namespace Kita {
    void Window::init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        KITA_ENGINE_INFO("Window initialized");
    }

    void Window::createWindow(const int width, const int height, const char* title) {
        m_window = glfwCreateWindow(width,height,title,nullptr,nullptr);
        glfwMakeContextCurrent(m_window);
        glfwSetFramebufferSizeCallback(m_window,frameBufferSizeCallbackFun);
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            KITA_ENGINE_ERROR("Failed to initialize Glad");
        }
        glfwSwapInterval(1);
        KITA_ENGINE_INFO("Window {} created",title);
    }

    void Window::errorCallbackFun(int error_code, const char* description) {
        KITA_ENGINE_ERROR("GLFW error: {}",description);
    }

    void Window::setErrorCallbackFun() {
        glfwSetErrorCallback(errorCallbackFun);
    }

    void Window::poolEvents() {
        glfwPollEvents();
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void Window::frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height) {
        glViewport(0,0,width,height);
    }

} // Kita