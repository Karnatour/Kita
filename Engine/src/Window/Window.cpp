#include "../kitapch.h"

#include "Window.h"

#include "Callbacks/KeyboardCallbacks.h"
#include "../Events/EventManager.h"
#include "Callbacks/MouseCallbacks.h"
#include "Callbacks/WindowCallbacks.h"

namespace Kita {
    void Window::init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        KITA_ENGINE_INFO("Window initialized");
    }

    void Window::createWindow(const int width, const int height, const char* title) {
        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        glfwMakeContextCurrent(m_window);
        setWindowCallbacks();
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            KITA_ENGINE_ERROR("Failed to initialize Glad");
        }
        glfwSwapInterval(1);
        KITA_ENGINE_INFO("Window {} created", title);
    }

    void Window::errorCallbackFun(int error_code, const char* description) {
        KITA_ENGINE_ERROR("GLFW error: {}", description);
    }

    void Window::frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
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

    void Window::setWindowCallbacks() {
        //Window
        glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallbackFun);
        glfwSetWindowPosCallback(m_window, windowPosCallbackFun);
        glfwSetWindowSizeCallback(m_window, windowSizeCallbackFun);
        glfwSetWindowCloseCallback(m_window, windowCloseCallbackFun);
        glfwSetWindowFocusCallback(m_window, windowFocusCallbackFun);

        //Keyboard
        glfwSetKeyCallback(m_window, keyboardKeyCallbackFun);

        //Mouse
        glfwSetMouseButtonCallback(m_window,mouseButtonCallbackFun);
        glfwSetCursorPosCallback(m_window,cursorPosCallbackFun);
        glfwSetCursorEnterCallback(m_window,cursorEnterCallbackFun);
        glfwSetScrollCallback(m_window,scrollCallbackFun);
    }
} // Kita
