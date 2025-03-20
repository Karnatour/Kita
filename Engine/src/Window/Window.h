#pragma once
#include <string>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Window {
    public:
        void init();
        void createWindow(int width, int height, const char* title);
        void exit();
        static void setErrorCallbackFun();
        void poolEvents();
        void swapBuffers();
        void makeContextCurrent();
        std::string m_title;
    private:
        void setWindowCallbacks();
        static void errorCallbackFun(int error_code, const char* description);
        static void frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height);
        GLFWwindow* m_window = nullptr;
    };
} // Kita
