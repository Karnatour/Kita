#include "kitapch.h"

#include "Window.h"
#include "Events/EventManager.h"
#include "Events/KeyboardEvents.h"
#include "Events/WindowEvents.h"

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

    void Window::windowPosCallbackFun(GLFWwindow* window, int xpos, int ypos) {
        WindowMoved event(xpos, ypos);
        EventManager::triggerEvent(event);
    }

    void Window::windowSizeCallbackFun(GLFWwindow* window, int width, int height) {
        WindowResized event(width, height);
        EventManager::triggerEvent(event);
    }

    void Window::windowCloseCallbackFun(GLFWwindow* window) {
        WindowClosed event;
        EventManager::triggerEvent(event);
    }

    void Window::windowFocusCallbackFun(GLFWwindow* window, int focused) {
        WindowChangedFocus event(focused);
        EventManager::triggerEvent(event);
    }

    void Window::windowKeyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods) {
        KeyPressed pressedEvent(convertGLFWToKitaKey(key),mods);
        KeyReleased releasedEvent(convertGLFWToKitaKey(key));
        switch (action) {
            case GLFW_PRESS:
                EventManager::triggerEvent(pressedEvent);
                break;
            case GLFW_RELEASE:
                EventManager::triggerEvent(releasedEvent);
                break;
            case GLFW_REPEAT:
                break;
        }
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
        glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallbackFun);
        glfwSetWindowPosCallback(m_window, windowPosCallbackFun);
        glfwSetWindowSizeCallback(m_window, windowSizeCallbackFun);
        glfwSetWindowCloseCallback(m_window, windowCloseCallbackFun);
        glfwSetWindowFocusCallback(m_window, windowFocusCallbackFun);
        glfwSetKeyCallback(m_window, windowKeyCallbackFun);
    }
} // Kita
