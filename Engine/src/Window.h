#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace Kita {
    class KITAENGINE_API Window {
    public:
        void init();
        void createWindow(int width, int height, const char* title);
        static void setErrorCallbackFun();
        void poolEvents();
        void swapBuffers();
    private:
        void setWindowCallbacks();
        static void errorCallbackFun(int error_code, const char* description);
        static void windowPosCallbackFun(GLFWwindow* window,int xpos, int ypos);
        static void windowSizeCallbackFun(GLFWwindow* window, int width, int height);
        static void windowCloseCallbackFun(GLFWwindow* window);
        static void windowFocusCallbackFun(GLFWwindow* window, int focused);
        static void windowKeyCallbackFun(GLFWwindow* window,int key,int scancode,int action,int mods);
        static void frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height);
        GLFWwindow* m_window = nullptr;
    };
} // Kita
