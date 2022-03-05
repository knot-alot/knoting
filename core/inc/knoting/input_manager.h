#pragma once
#include <knoting/types.h>

struct GLFWwindow;

namespace knot{

    class Window;

    enum class MouseButtonCode {
        Left = 0,
        Right = 1,
        Middle = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,
        Button8 = 8,
        Last
    };

    enum class JoyStickCode {
        LeftX = 0,
        LeftY = 1,
        RightX = 2,
        RightY = 3,
        LeftBumper = 4,
        RightBumper = 5,
        Last };

    enum class PadCode {
        Pad0 = 0,
        Pad1 = 1,
        Pad2 = 2,
        Pad3 = 3,
        Pad4 = 4,
        Pad5 = 5,
        Pad6 = 6,
        Pad7 = 7,
        Pad8 = 8,
        Pad9 = 9,
        Pad10 = 10,
        Pad11 = 11,
        Pad12 = 12,
        Pad13 = 13,
        Pad14 = 14,
        Pad15 = 15,
        Last
    };

    enum class PadButtonCode {
        A = 0,
        B = 1,
        X = 2,
        Y = 3,
        LeftBumper = 4,
        RightBumper = 5,
        Back = 6,
        Start = 7,
        Guide = 8,
        LeftThumb = 9,
        RightThumb = 10,
        DPadUp = 11,
        DPadRight = 12,
        DPadDown = 13,
        DPadLeft = 14,
        Last
    };

    enum class KeyCode {
        Unknown = -1,
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        N0 = 48,
        N1 = 49,
        N2 = 50,
        N3 = 51,
        N4 = 52,
        N5 = 53,
        N6 = 54,
        N7 = 55,
        N8 = 56,
        N9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,
        World1 = 161,
        World2 = 162,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        F13 = 302,
        F14 = 303,
        F15 = 304,
        F16 = 305,
        F17 = 306,
        F18 = 307,
        F19 = 308,
        F20 = 309,
        F21 = 310,
        F22 = 311,
        F23 = 312,
        F24 = 313,
        F25 = 314,
        KP0 = 320,
        KP1 = 321,
        KP2 = 322,
        KP3 = 323,
        KP4 = 324,
        KP5 = 325,
        KP6 = 326,
        KP7 = 327,
        KP8 = 328,
        KP9 = 329,
        KPDecimal = 330,
        KPDivide = 331,
        KPMultiply = 332,
        KPSubtract = 333,
        KPAdd = 334,
        KPEnter = 335,
        KPEqual = 336,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348,
        Last
    };

    class input_manager {
       public:
        ///KeyBoard Methods
        bool m_keyPressed(KeyCode key);
        bool m_keyHeldDown(KeyCode key);
        bool m_keyOnTrigger(KeyCode key);
        bool m_keyOnRelease(KeyCode key);

        ///Pad Methods
        bool  m_padButtonPressed(PadCode pad, PadButtonCode button);
        bool  m_padButtonHeldDown(PadCode pad, PadButtonCode button);
        bool  m_padButtonTriggered(PadCode pad, PadButtonCode button);
        bool  m_padButtonReleased(PadCode pad, PadButtonCode button);
        float m_getPadAxis(PadCode pad, JoyStickCode joy_stick);
        bool  m_padPresent(PadCode pad);

        ///Mouse Methods
        bool m_mouseButtonPressed(MouseButtonCode button);
        bool m_mouseButtonHeldDown(MouseButtonCode button);
        bool m_mouseButtonTriggered(MouseButtonCode button);
        bool m_mouseButtonReleased(MouseButtonCode button);
        bool m_mouseDoubleClicked(MouseButtonCode button);
        void SetDoubleClickLimit(float ms);
        void m_setMouseSensitivity(float sensitivity);
        bool m_wheelMoved();
        vec2 m_getRelativePosition();
        vec2 m_getAbsolutePosition();
        vec2 m_getWheelMovement();



       protected:
        friend Window;

        input_manager(Window &owner);
        ~input_manager();

        void m_updatePads(GLFWwindow *glfwWindow);
        void m_keyEvent(int key, bool pressed);
        void m_mouseButtonEvent(int button, bool pressed);
        void m_mouseEvent(double x, double y);
        void m_updateHolds();
        void m_updateRelativePositions();
        void m_scrollEvent(vec2 offset);

        bool m_keyBinding[(int)KeyCode::Last];
        bool m_mouseBindings[(int)MouseButtonCode::Last];
        bool m_padBindings[(int)PadCode::Last][(int)PadButtonCode::Last];
        bool m_holdKeyBindings[(int)KeyCode::Last];
        bool m_holdMouseBindings[(int)MouseButtonCode::Last];
        bool m_holdPadBindings[(int)PadCode::Last][(int)PadButtonCode::Last];
        vec2 m_lastScroll;
        vec<2,double> m_mousePosition;
        vec<2,double> m_lastMousePosition;
        vec<2,double> m_relativeMousePosition;
        float m_joyStickBindings[(int)PadCode::Last][(int)JoyStickCode::Last];
        float m_sensitivity;
    };

} // namespace knot
