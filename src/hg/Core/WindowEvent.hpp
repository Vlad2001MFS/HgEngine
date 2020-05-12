#pragma once
#include <SDL2/SDL_events.h>

namespace hg {

enum class WindowEventType {
    Close,
    Resize,
    FocusLost,
    FocusGained,
    Key,
    MouseWheel,
    MouseButton,
    MouseMove
};

enum class MouseButton {
    Left,
    Right,
    Middle,
    X1,
    X2
};

enum class KeyCode {
    Unknown,
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
    Return, Escape, BackSpace, Tab, Space,
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    Pause, Insert, Home, PageUp, PageDown, Delete, End, Left, Right, Up, Down,
    Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
    LControl, RControl, LShift, RShift, LAlt, RAlt, LSystem, RSystem
};

enum class KeyState {
    Pressed,
    Released
};

struct ResizeWindowEvent {
    int w, h;
};

struct KeyboardWindowEvent {
    KeyCode code;
    KeyState state;
    bool alt;
    bool ctrl;
    bool shift;
    bool system;
};

struct MouseWheelWindowEvent {
    int x; // if greater then zero - scroll right, else if less then zero - scroll left
    int y; // if greater then zero - scroll up, else if less then zero - scroll down
    bool leftBtn;
    bool middleBtn;
    bool rightBtn;
    bool xBtn1;
    bool xBtn2;
    bool ctrl;
    bool shift;
};

struct MouseButtonWindowEvent final {
    MouseButton btn;
    KeyState state;
    int x, y;
};

struct MouseMoveWindowEvent {
    int x, y;
    int deltaX, deltaY;
};

struct WindowEvent {
    WindowEventType type;
    union {
        ResizeWindowEvent resize;
        KeyboardWindowEvent key;
        MouseWheelWindowEvent mouseWheel;
        MouseButtonWindowEvent mouseButton;
        MouseMoveWindowEvent mouseMove;
    };
    SDL_Event sdlEvent;
};

}
