#pragma once
#include <string>

#define GLFW_INCLUDE_NONE
// ReSharper disable once CppUnusedIncludeDirective
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/DllTemplate.h"
#include "../Events/WindowEvents.h"

enum class CursorMode {
    DISABLED,
    HIDDEN,
    SHOWN
};

namespace Kita {
    class KITAENGINE_API Window {
    public:
        static constexpr std::string_view GLSL_VERSION = "#version 460";

        void init();
        void createWindow(int width, int height, const char* title);
        void exit();
        static double getTime();
        void poolEvents();
        void swapBuffers();
        void makeContextCurrent();

        GLFWwindow* getGLFWwindow() const;

        std::pair<float, float> getMonitorScale() const;
        std::pair<int, int> getWindowResolution() const;
        std::pair<int, int> getFrameBufferResolution() const;

        std::string getTitle();
        void setTitle(const std::string& title);

        void setCursorMode(CursorMode mode);
        CursorMode getCursorMode() const;

    private:
        void setWindowPosToCenter() const;
        static void setErrorCallbackFun();
        void setWindowCallbacks();
        static void errorCallbackFun(int error_code, const char* description);
        void setDarkMode();

        std::string m_title;
        GLFWwindow* m_window = nullptr;
        CursorMode m_cursorMode = CursorMode::SHOWN;
        std::pair<int, int> m_windowResolution;
        std::pair<int, int> m_frameBufferResolution;

        static void updateWindowResolution(const WindowResized& event);
        static void updateFrameBufferResolution(const FrameBufferResized& event);
    };
} // Kita
