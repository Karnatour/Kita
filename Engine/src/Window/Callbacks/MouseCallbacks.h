#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Kita {
    void mouseButtonCallbackFun(GLFWwindow* window, int button, int action, int mods);
    void cursorPosCallbackFun(GLFWwindow* window, double xpos, double ypos);
    void cursorEnterCallbackFun(GLFWwindow* window, int entered);
    void scrollCallbackFun(GLFWwindow* window, double xoffset, double yoffset);
}
