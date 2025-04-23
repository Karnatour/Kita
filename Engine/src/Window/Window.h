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
        static double getTime();
        void poolEvents();
        void swapBuffers();
        void makeContextCurrent();
        std::string getTitle();
        std::pair<int,int> getResolution();
        void setTitle(const std::string& title);

    private:
        void setWindowCallbacks();
        static void errorCallbackFun(int error_code, const char* description);
        static void frameBufferSizeCallbackFun(GLFWwindow* window, int width, int height);
        std::string m_title;
        GLFWwindow* m_window = nullptr;
        std::pair<int,int> m_resolution;
    };
} // Kita
