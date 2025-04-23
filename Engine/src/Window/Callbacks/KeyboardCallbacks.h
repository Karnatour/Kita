#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Kita {
    void keyboardKeyCallbackFun(GLFWwindow* window, int key, int scancode, int action, int mods);
}
