#include "Window.h"

#include "../Core/Engine.h"
#include "Callbacks/KeyboardCallbacks.h"
#include "../Events/EventManager.h"
#include "Callbacks/MouseCallbacks.h"
#include "Callbacks/WindowCallbacks.h"

namespace Kita {
    void Window::init() {
        setErrorCallbackFun();
        if (!glfwInit()) {
            KITA_ENGINE_ERROR("GLFW initialization failed!");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        KITA_ENGINE_INFO("Window initialized");
    }

    void Window::createWindow(const int width, const int height, const char* title) {
        m_resolution = std::make_pair(width, height);
        KITA_ENGINE_INFO("Creating window: {} ({}x{})", title, width, height);

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window) {
            KITA_ENGINE_ERROR("Failed to create GLFW window");
            return;
        }

        setWindowPosToCenter();

        glfwSetWindowUserPointer(m_window, this);

        makeContextCurrent();
        setWindowCallbacks();

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            KITA_ENGINE_ERROR("Failed to initialize Glad");
            return;
        }

        glfwSwapInterval(1);

        KITA_ENGINE_INFO("Window {} created", title);
    }

    void Window::exit() {
        glfwTerminate();
        KITA_ENGINE_INFO("GLFW terminated");
    }

    void Window::errorCallbackFun(int error_code, const char* description) {
        KITA_ENGINE_ERROR("GLFW error: {}", description);
    }

    void Window::frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height) {
        Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (window) {
            windowPtr->m_resolution.first = width;
            windowPtr->m_resolution.second = height;
        }

        Engine::getEngine()->getRenderer().getRendererAPI().setViewport(width, height, true);
    }

    void Window::setErrorCallbackFun() {
        glfwSetErrorCallback(errorCallbackFun);
    }

    double Window::getTime() {
        return glfwGetTime();
    }

    void Window::poolEvents() {
        glfwPollEvents();
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_window);
    }

    void Window::makeContextCurrent() {
        glfwMakeContextCurrent(m_window);
    }

    std::string Window::getTitle() {
        return m_title;
    }

    std::pair<int, int> Window::getResolution() const {
        return m_resolution;
    }

    void Window::setCursorMode(const CursorMode mode) {
        if (m_cursorMode == mode) {
            return;
        }
        m_cursorMode = mode;
        switch (mode) {
            case CursorMode::DISABLED:
                glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
                break;
            case CursorMode::HIDDEN:
                glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
                break;
            case CursorMode::SHOWN:
                glfwSetInputMode(m_window,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
                break;
        }
    }

    CursorMode Window::getCursorMode() const {
        return m_cursorMode;
    }

    void Window::setWindowPosToCenter() const {
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        int monitorX, monitorY;
        glfwGetMonitorPos(primary, &monitorX, &monitorY);

        const GLFWvidmode* mode = glfwGetVideoMode(primary);
        const int monitorWidth = mode->width;
        const int monitorHeight = mode->height;

        int windowWidth, windowHeight;
        glfwGetWindowSize(m_window, &windowWidth, &windowHeight);

        const int windowPosX = monitorX + (monitorWidth - windowWidth) / 2;
        const int windowPosY = monitorY + (monitorHeight - windowHeight) / 2;

        glfwSetWindowPos(m_window, windowPosX, windowPosY);
    }

    void Window::setTitle(const std::string& title) {
        m_title = title;
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
        glfwSetMouseButtonCallback(m_window, mouseButtonCallbackFun);
        glfwSetCursorPosCallback(m_window, cursorPosCallbackFun);
        glfwSetCursorEnterCallback(m_window, cursorEnterCallbackFun);
        glfwSetScrollCallback(m_window, scrollCallbackFun);
    }
} // Kita
