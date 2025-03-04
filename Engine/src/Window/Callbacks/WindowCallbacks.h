#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Kita {
    void windowPosCallbackFun(GLFWwindow* window, int xpos, int ypos);
    void windowSizeCallbackFun(GLFWwindow* window, int width, int height);
    void windowCloseCallbackFun(GLFWwindow* window);
    void windowFocusCallbackFun(GLFWwindow* window, int focused);
}
