#include "InputKeys.h"
#include <GLFW/glfw3.h>

namespace Kita {
    MouseButton convertGLFWToKitaButton(const int button) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_1:
            return MouseButton::MBUTTON_LEFT;
        case GLFW_MOUSE_BUTTON_2:
            return MouseButton::MBUTTON_RIGHT;
        case GLFW_MOUSE_BUTTON_3:
            return MouseButton::MBUTTON_MIDDLE;
        case GLFW_MOUSE_BUTTON_4:
            return MouseButton::MBUTTON_4;
        case GLFW_MOUSE_BUTTON_5:
            return MouseButton::MBUTTON_5;
        case GLFW_MOUSE_BUTTON_6:
            return MouseButton::MBUTTON_6;
        case GLFW_MOUSE_BUTTON_7:
            return MouseButton::MBUTTON_7;
        case GLFW_MOUSE_BUTTON_8:
            return MouseButton::MBUTTON_LAST;
        default:
            return MouseButton::MBUTTON_NONE;
        }
    }

    KeyboardKey Kita::convertGLFWToKitaKey(const int key) {
        switch (key) {
        case GLFW_KEY_SPACE:
            return KeyboardKey::KEY_SPACE;
        case GLFW_KEY_APOSTROPHE:
            return KeyboardKey::KEY_APOSTROPHE;
        case GLFW_KEY_COMMA:
            return KeyboardKey::KEY_COMMA;
        case GLFW_KEY_MINUS:
            return KeyboardKey::KEY_MINUS;
        case GLFW_KEY_PERIOD:
            return KeyboardKey::KEY_PERIOD;
        case GLFW_KEY_SLASH:
            return KeyboardKey::KEY_SLASH;
        case GLFW_KEY_0:
            return KeyboardKey::KEY_0;
        case GLFW_KEY_1:
            return KeyboardKey::KEY_1;
        case GLFW_KEY_2:
            return KeyboardKey::KEY_2;
        case GLFW_KEY_3:
            return KeyboardKey::KEY_3;
        case GLFW_KEY_4:
            return KeyboardKey::KEY_4;
        case GLFW_KEY_5:
            return KeyboardKey::KEY_5;
        case GLFW_KEY_6:
            return KeyboardKey::KEY_6;
        case GLFW_KEY_7:
            return KeyboardKey::KEY_7;
        case GLFW_KEY_8:
            return KeyboardKey::KEY_8;
        case GLFW_KEY_9:
            return KeyboardKey::KEY_9;
        case GLFW_KEY_SEMICOLON:
            return KeyboardKey::KEY_SEMICOLON;
        case GLFW_KEY_EQUAL:
            return KeyboardKey::KEY_EQUAL;
        case GLFW_KEY_A:
            return KeyboardKey::KEY_A;
        case GLFW_KEY_B:
            return KeyboardKey::KEY_B;
        case GLFW_KEY_C:
            return KeyboardKey::KEY_C;
        case GLFW_KEY_D:
            return KeyboardKey::KEY_D;
        case GLFW_KEY_E:
            return KeyboardKey::KEY_E;
        case GLFW_KEY_F:
            return KeyboardKey::KEY_F;
        case GLFW_KEY_G:
            return KeyboardKey::KEY_G;
        case GLFW_KEY_H:
            return KeyboardKey::KEY_H;
        case GLFW_KEY_I:
            return KeyboardKey::KEY_I;
        case GLFW_KEY_J:
            return KeyboardKey::KEY_J;
        case GLFW_KEY_K:
            return KeyboardKey::KEY_K;
        case GLFW_KEY_L:
            return KeyboardKey::KEY_L;
        case GLFW_KEY_M:
            return KeyboardKey::KEY_M;
        case GLFW_KEY_N:
            return KeyboardKey::KEY_N;
        case GLFW_KEY_O:
            return KeyboardKey::KEY_O;
        case GLFW_KEY_P:
            return KeyboardKey::KEY_P;
        case GLFW_KEY_Q:
            return KeyboardKey::KEY_Q;
        case GLFW_KEY_R:
            return KeyboardKey::KEY_R;
        case GLFW_KEY_S:
            return KeyboardKey::KEY_S;
        case GLFW_KEY_T:
            return KeyboardKey::KEY_T;
        case GLFW_KEY_U:
            return KeyboardKey::KEY_U;
        case GLFW_KEY_V:
            return KeyboardKey::KEY_V;
        case GLFW_KEY_W:
            return KeyboardKey::KEY_W;
        case GLFW_KEY_X:
            return KeyboardKey::KEY_X;
        case GLFW_KEY_Y:
            return KeyboardKey::KEY_Y;
        case GLFW_KEY_Z:
            return KeyboardKey::KEY_Z;
        case GLFW_KEY_LEFT_BRACKET:
            return KeyboardKey::KEY_LEFT_BRACKET;
        case GLFW_KEY_BACKSLASH:
            return KeyboardKey::KEY_BACKSLASH;
        case GLFW_KEY_RIGHT_BRACKET:
            return KeyboardKey::KEY_RIGHT_BRACKET;
        case GLFW_KEY_GRAVE_ACCENT:
            return KeyboardKey::KEY_GRAVE_ACCENT;
        case GLFW_KEY_WORLD_1:
            return KeyboardKey::KEY_WORLD_1;
        case GLFW_KEY_WORLD_2:
            return KeyboardKey::KEY_WORLD_2;
        case GLFW_KEY_ESCAPE:
            return KeyboardKey::KEY_ESCAPE;
        case GLFW_KEY_ENTER:
            return KeyboardKey::KEY_ENTER;
        case GLFW_KEY_TAB:
            return KeyboardKey::KEY_TAB;
        case GLFW_KEY_BACKSPACE:
            return KeyboardKey::KEY_BACKSPACE;
        case GLFW_KEY_INSERT:
            return KeyboardKey::KEY_INSERT;
        case GLFW_KEY_DELETE:
            return KeyboardKey::KEY_DELETE;
        case GLFW_KEY_RIGHT:
            return KeyboardKey::KEY_RIGHT;
        case GLFW_KEY_LEFT:
            return KeyboardKey::KEY_LEFT;
        case GLFW_KEY_DOWN:
            return KeyboardKey::KEY_DOWN;
        case GLFW_KEY_UP:
            return KeyboardKey::KEY_UP;
        case GLFW_KEY_PAGE_UP:
            return KeyboardKey::KEY_PAGE_UP;
        case GLFW_KEY_PAGE_DOWN:
            return KeyboardKey::KEY_PAGE_DOWN;
        case GLFW_KEY_HOME:
            return KeyboardKey::KEY_HOME;
        case GLFW_KEY_END:
            return KeyboardKey::KEY_END;
        case GLFW_KEY_CAPS_LOCK:
            return KeyboardKey::KEY_CAPS_LOCK;
        case GLFW_KEY_SCROLL_LOCK:
            return KeyboardKey::KEY_SCROLL_LOCK;
        case GLFW_KEY_NUM_LOCK:
            return KeyboardKey::KEY_NUM_LOCK;
        case GLFW_KEY_PRINT_SCREEN:
            return KeyboardKey::KEY_PRINT_SCREEN;
        case GLFW_KEY_PAUSE:
            return KeyboardKey::KEY_PAUSE;
        case GLFW_KEY_F1:
            return KeyboardKey::KEY_F1;
        case GLFW_KEY_F2:
            return KeyboardKey::KEY_F2;
        case GLFW_KEY_F3:
            return KeyboardKey::KEY_F3;
        case GLFW_KEY_F4:
            return KeyboardKey::KEY_F4;
        case GLFW_KEY_F5:
            return KeyboardKey::KEY_F5;
        case GLFW_KEY_F6:
            return KeyboardKey::KEY_F6;
        case GLFW_KEY_F7:
            return KeyboardKey::KEY_F7;
        case GLFW_KEY_F8:
            return KeyboardKey::KEY_F8;
        case GLFW_KEY_F9:
            return KeyboardKey::KEY_F9;
        case GLFW_KEY_F10:
            return KeyboardKey::KEY_F10;
        case GLFW_KEY_F11:
            return KeyboardKey::KEY_F11;
        case GLFW_KEY_F12:
            return KeyboardKey::KEY_F12;
        case GLFW_KEY_F13:
            return KeyboardKey::KEY_F13;
        case GLFW_KEY_F14:
            return KeyboardKey::KEY_F14;
        case GLFW_KEY_F15:
            return KeyboardKey::KEY_F15;
        case GLFW_KEY_F16:
            return KeyboardKey::KEY_F16;
        case GLFW_KEY_F17:
            return KeyboardKey::KEY_F17;
        case GLFW_KEY_F18:
            return KeyboardKey::KEY_F18;
        case GLFW_KEY_F19:
            return KeyboardKey::KEY_F19;
        case GLFW_KEY_F20:
            return KeyboardKey::KEY_F20;
        case GLFW_KEY_F21:
            return KeyboardKey::KEY_F21;
        case GLFW_KEY_F22:
            return KeyboardKey::KEY_F22;
        case GLFW_KEY_F23:
            return KeyboardKey::KEY_F23;
        case GLFW_KEY_F24:
            return KeyboardKey::KEY_F24;
        case GLFW_KEY_F25:
            return KeyboardKey::KEY_F25;
        case GLFW_KEY_KP_0:
            return KeyboardKey::KEY_KP_0;
        case GLFW_KEY_KP_1:
            return KeyboardKey::KEY_KP_1;
        case GLFW_KEY_KP_2:
            return KeyboardKey::KEY_KP_2;
        case GLFW_KEY_KP_3:
            return KeyboardKey::KEY_KP_3;
        case GLFW_KEY_KP_4:
            return KeyboardKey::KEY_KP_4;
        case GLFW_KEY_KP_5:
            return KeyboardKey::KEY_KP_5;
        case GLFW_KEY_KP_6:
            return KeyboardKey::KEY_KP_6;
        case GLFW_KEY_KP_7:
            return KeyboardKey::KEY_KP_7;
        case GLFW_KEY_KP_8:
            return KeyboardKey::KEY_KP_8;
        case GLFW_KEY_KP_9:
            return KeyboardKey::KEY_KP_9;
        case GLFW_KEY_KP_DECIMAL:
            return KeyboardKey::KEY_KP_DECIMAL;
        case GLFW_KEY_KP_DIVIDE:
            return KeyboardKey::KEY_KP_DIVIDE;
        case GLFW_KEY_KP_MULTIPLY:
            return KeyboardKey::KEY_KP_MULTIPLY;
        case GLFW_KEY_KP_SUBTRACT:
            return KeyboardKey::KEY_KP_SUBTRACT;
        case GLFW_KEY_KP_ADD:
            return KeyboardKey::KEY_KP_ADD;
        case GLFW_KEY_KP_ENTER:
            return KeyboardKey::KEY_KP_ENTER;
        case GLFW_KEY_KP_EQUAL:
            return KeyboardKey::KEY_KP_EQUAL;
        case GLFW_KEY_LEFT_SHIFT:
            return KeyboardKey::KEY_LEFT_SHIFT;
        case GLFW_KEY_LEFT_CONTROL:
            return KeyboardKey::KEY_LEFT_CONTROL;
        case GLFW_KEY_LEFT_ALT:
            return KeyboardKey::KEY_LEFT_ALT;
        case GLFW_KEY_LEFT_SUPER:
            return KeyboardKey::KEY_LEFT_SUPER;
        case GLFW_KEY_RIGHT_SHIFT:
            return KeyboardKey::KEY_RIGHT_SHIFT;
        case GLFW_KEY_RIGHT_CONTROL:
            return KeyboardKey::KEY_RIGHT_CONTROL;
        case GLFW_KEY_RIGHT_ALT:
            return KeyboardKey::KEY_RIGHT_ALT;
        case GLFW_KEY_RIGHT_SUPER:
            return KeyboardKey::KEY_RIGHT_SUPER;
        case GLFW_KEY_MENU:
            return KeyboardKey::KEY_MENU;
        default:
            return KeyboardKey::KEY_NONE;
        }
    }
}
