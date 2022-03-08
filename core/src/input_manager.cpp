#include <GLFW/glfw3.h>
#include <knoting/input_manager.h>
#include <knoting/window.h>
#include <algorithm>

namespace knot {

InputManager::InputManager(Window &owner) : m_sensitivity(0.3f), m_lastScroll(0.0f) {
    for (int i = 0; i < (int)KeyCode::Last; ++i) {
        m_keyBinding[i] = false;
        m_holdKeyBindings[i] = false;
    }

    for (int i = 0; i < (int)MouseButtonCode::Last; ++i) {
        m_mouseBindings[i] = false;
        m_holdMouseBindings[i] = false;
    }

    for (int i = 0; i < (int)PadCode::Last; ++i) {
        for (int j = 0; j < (int)PadButtonCode::Last; ++j) {
            m_padBindings[i][j] = false;
            m_holdKeyBindings[i] = false;
        }

        for (int j = 0; j < (int)JoyStickCode::Last; ++j) {
            m_joyStickBindings[i][j] = 0.0f;
        }
    }
}

InputManager::~InputManager() {}

/// Start Key Keyboard
bool InputManager::key_pressed(KeyCode key) {
    return m_keyBinding[(int)key];
}

    bool InputManager::key_held_down(KeyCode key) {
        return m_keyBinding[(int)key] && m_holdKeyBindings[(int)key];
    }

    bool InputManager::key_on_trigger(KeyCode key) {
        return m_keyBinding[(int)key] && !m_holdKeyBindings[(int)key];
}

bool InputManager::key_on_release(KeyCode key) {
    return m_keyBinding[(int)key] && !m_holdKeyBindings[(int)key];
}

/// End Of Keyboard

/// Start Of Pad
bool InputManager::pad_present(PadCode pad) {
    return glfwJoystickPresent((int)pad);
}

bool InputManager::pad_button_pressed(PadCode pad, PadButtonCode button) {
    return m_padBindings[(int)pad][(int)button];
}

bool InputManager::pad_button_held_down(PadCode pad, PadButtonCode button) {
    return m_padBindings[(int)pad][(int)button] && m_holdPadBindings[(int)pad][(int)button];
}

bool InputManager::pad_button_triggered(PadCode pad, PadButtonCode button) {
    return m_padBindings[(int)pad][(int)button] && !m_holdPadBindings[(int)pad][(int)button];
}

bool InputManager::pad_button_released(PadCode pad, PadButtonCode button) {
    return m_padBindings[(int)pad][(int)button] && !m_holdPadBindings[(int)pad][(int)button];
}

float InputManager::get_pad_axis(PadCode pad, JoyStickCode joy_stick) {
    return m_joyStickBindings[(int)pad][(int)joy_stick];
}

void InputManager::update_pads(GLFWwindow* glfwWindow) {
    for (int i = 0; i < (int)PadCode::Last; i++) {
        if (!pad_present((PadCode)i))
            continue;

        int count;
        const float* axes = glfwGetJoystickAxes(i, &count);
        if (!axes) continue;

        if (count > (int)JoyStickCode::Last)
            count = (int)JoyStickCode::Last;
        for (int n = 0; n < count; ++n)
            m_joyStickBindings[i][n] = axes[n];

        GLFWgamepadstate gameState;
        if (glfwGetGamepadState(i, &gameState)) {
            for (int j = 0; j < (int)PadButtonCode::Last; j++) {
                bool padPressed = gameState.buttons[j];
                m_padBindings[i][j] = padPressed;
            }
        } else if (glfwJoystickPresent(i)) {
            const unsigned char* buttonPressed = glfwGetJoystickButtons(i, &count);
            if (!buttonPressed) continue;
            for (int j = 0; j < count; ++j) {
                if (count > 0)
                    m_padBindings[i][(int)PadButtonCode::A] = buttonPressed[0];
                if (count > 1)
                    m_padBindings[i][(int)PadButtonCode::B] = buttonPressed[1];
                if (count > 3)
                    m_padBindings[i][(int)PadButtonCode::X] = buttonPressed[3];
                if (count > 4)
                    m_padBindings[i][(int)PadButtonCode::Y] = buttonPressed[4];
                if (count > 6)
                    m_padBindings[i][(int)PadButtonCode::LeftBumper] = buttonPressed[6];
                if (count > 7)
                    m_padBindings[i][(int)PadButtonCode::RightBumper] = buttonPressed[7];
                if (count > 11)
                    m_padBindings[i][(int)PadButtonCode::Start] = buttonPressed[11];
                if (count > 13)
                    m_padBindings[i][(int)PadButtonCode::LeftThumb] = buttonPressed[13];
                if (count > 14)
                    m_padBindings[i][(int)PadButtonCode::RightThumb] = buttonPressed[14];
                if (count > 15)
                    m_padBindings[i][(int)PadButtonCode::DPadUp] = buttonPressed[15];
                if (count > 16)
                    m_padBindings[i][(int)PadButtonCode::DPadRight] = buttonPressed[16];
                if (count > 17)
                    m_padBindings[i][(int)PadButtonCode::DPadDown] = buttonPressed[17];
                if (count > 18)
                    m_padBindings[i][(int)PadButtonCode::DPadLeft] = buttonPressed[18];
            }
        }
    }
}
/// End Of Pad

/// Start Of Mouse
bool InputManager::mouse_button_pressed(MouseButtonCode button) {
    return m_mouseBindings[(int)button];
}

bool InputManager::mouse_button_held_down(MouseButtonCode button) {
    return m_mouseBindings[(int)button] && m_holdMouseBindings[(int)button];
}

bool InputManager::mouse_button_triggered(MouseButtonCode button) {
    return m_mouseBindings[(int)button] && !m_holdMouseBindings[(int)button];
}

bool InputManager::mouse_button_released(MouseButtonCode button) {
    return !m_mouseBindings[(int)button] && m_holdMouseBindings[(int)button];
}

bool InputManager::mouse_double_clicked(MouseButtonCode button) {
    return false;
}

bool InputManager::wheel_moved() {
    return m_lastScroll != vec2{};
}

vec2 InputManager::get_relative_position() {
    return vec2(m_relativeMousePosition.x * m_sensitivity, m_relativeMousePosition.y * m_sensitivity);
}

vec2 InputManager::get_absolute_position() {
    return vec2(m_mousePosition.x, m_mousePosition.y);
}

vec2 InputManager::get_wheel_movement() {
    return m_lastScroll;
}

void InputManager::set_mouse_sensitivity(float sens) {
    m_sensitivity = sens;
}

void InputManager::update_relative_positions() {
    m_relativeMousePosition = m_mousePosition - m_lastMousePosition;
    m_lastMousePosition = m_mousePosition;
}
/// End of Mouse

/// Start of Events
void InputManager::scroll_event(vec2 offset) {
    m_lastScroll = offset;
}

void InputManager::key_event(int key, bool pressed) {
    m_keyBinding[key] = pressed;
}
void InputManager::mouse_button_event(int button, bool pressed) {
    m_mouseBindings[button] = pressed;
}
void InputManager::mouse_event(double x, double y) {
    m_mousePosition.x = x;
    m_mousePosition.y = y;
}
/// End of Events

void InputManager::update_holds() {
    std::copy_n(m_keyBinding, (int)KeyCode::Last, m_holdKeyBindings);
    std::copy_n(m_mouseBindings, (int)MouseButtonCode::Last, m_holdMouseBindings);
    for (int i = 0; i < (int)PadCode::Last; ++i)
        std::copy_n(m_padBindings, (int)PadCode::Last, m_holdMouseBindings);
    m_lastScroll = vec2(0.0f);
}
}  // namespace knot