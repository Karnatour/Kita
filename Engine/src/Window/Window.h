#pragma once
#include <string>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/DllTemplate.h"

enum class CursorMode {
    DISABLED,
    HIDDEN,
    SHOWN
};

namespace Kita {
    class KITAENGINE_API Window {
    public:
        void init();
        void createWindow(int width, int height, const char* title);
        void exit();
        static double getTime();
        void poolEvents();
        void swapBuffers();
        void makeContextCurrent();

        std::pair<int,int> getResolution() const;

        std::string getTitle();
        void setTitle(const std::string& title);

        void setCursorMode(CursorMode mode);
        CursorMode getCursorMode() const;

    private:
        void setWindowPosToCenter() const;
        static void setErrorCallbackFun();
        void setWindowCallbacks();
        static void errorCallbackFun(int error_code, const char* description);
        static void frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height);

        std::string m_title;
        GLFWwindow* m_window = nullptr;
        CursorMode m_cursorMode = CursorMode::SHOWN;
        std::pair<int,int> m_resolution;
    };
} // Kita
