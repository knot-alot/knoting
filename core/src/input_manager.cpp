#include <GLFW/glfw3.h>
#include <knoting/window.h>
#include <algorithm>
#include <iostream>

namespace knot {

InputManager::InputManager(Window& owner) : m_sensitivity(0.3f), m_lastScroll(0.0f) {
    m_keyBinding.fill(false);
    m_holdKeyBindings.fill(false);

    m_mouseBindings.fill(false);
    m_holdMouseBindings.fill(false);

    for (int i = 0; i < static_cast<size_t>(PadCode::Last); ++i) {
        m_padBindings[i].fill(false);
        m_holdPadBindings[i].fill(false);
        m_joyStickBindings[i].fill(0.0f);
    }
}

InputManager::~InputManager() {}

/// Start Key Keyboard
bool InputManager::key_pressed(KeyCode key) {
    return m_keyBinding[static_cast<size_t>(key)];
}

bool InputManager::key_held_down(KeyCode key) {
    return m_keyBinding[static_cast<size_t>(key)] && m_holdKeyBindings[static_cast<size_t>(key)];
}

bool InputManager::key_on_trigger(KeyCode key) {
    return m_keyBinding[static_cast<size_t>(key)] && !m_holdKeyBindings[static_cast<size_t>(key)];
}

bool InputManager::key_on_release(KeyCode key) {
    return m_keyBinding[static_cast<size_t>(key)] && !m_holdKeyBindings[static_cast<size_t>(key)];
}

/// End Of Keyboard

/// Start Of Pad
/// Issues with the pads im still working on them.
bool InputManager::pad_present(PadCode pad) {
    return glfwJoystickPresent(static_cast<size_t>(pad));
}

bool InputManager::pad_button_pressed(PadCode pad, PadButtonCode button) {
    return m_padBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)];
}

bool InputManager::pad_button_held_down(PadCode pad, PadButtonCode button) {
    return m_padBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)] &&
           m_holdPadBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)];
}

bool InputManager::pad_button_triggered(PadCode pad, PadButtonCode button) {
    return m_padBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)] &&
           !m_holdPadBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)];
}

bool InputManager::pad_button_released(PadCode pad, PadButtonCode button) {
    return m_padBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)] &&
           !m_holdPadBindings[static_cast<size_t>(pad)][static_cast<size_t>(button)];
}

float InputManager::get_pad_axis(PadCode pad, JoyStickCode joy_stick) {
    return m_joyStickBindings[static_cast<size_t>(pad)][static_cast<size_t>(joy_stick)];
}

void InputManager::update_pads(GLFWwindow* glfwWindow) {
    for (int i = 0; i < static_cast<size_t>(PadCode::Last); i++) {
        if (!pad_present((PadCode)i))
            continue;

        int count;
        const float* axes = glfwGetJoystickAxes(i, &count);
        if (!axes)
            continue;

        if (count > static_cast<size_t>(JoyStickCode::Last))
            count = static_cast<size_t>(JoyStickCode::Last);
        for (int n = 0; n < count; ++n)
            m_joyStickBindings[i][n] = axes[n];

        GLFWgamepadstate gameState;
        if (glfwGetGamepadState(i, &gameState)) {
            for (int j = 0; j < static_cast<size_t>(PadButtonCode::Last); j++) {
                bool padPressed = gameState.buttons[j];
                m_padBindings[i][j] = padPressed;
            }
        } else if (glfwJoystickPresent(i)) {
            const unsigned char* buttonPressed = glfwGetJoystickButtons(i, &count);
            if (!buttonPressed)
                continue;
            if (count > 0)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::A)] = buttonPressed[0];
            if (count > 1)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::B)] = buttonPressed[1];
            if (count > 3)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::X)] = buttonPressed[3];
            if (count > 4)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::Y)] = buttonPressed[4];
            if (count > 6)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::LeftBumper)] = buttonPressed[6];
            if (count > 7)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::RightBumper)] = buttonPressed[7];
            if (count > 11)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::Start)] = buttonPressed[11];
            if (count > 13)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::LeftThumb)] = buttonPressed[13];
            if (count > 14)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::RightThumb)] = buttonPressed[14];
            if (count > 15)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::DPadUp)] = buttonPressed[15];
            if (count > 16)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::DPadRight)] = buttonPressed[16];
            if (count > 17)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::DPadDown)] = buttonPressed[17];
            if (count > 18)
                m_padBindings[i][static_cast<size_t>(PadButtonCode::DPadLeft)] = buttonPressed[18];
        }
    }
}
/// End Of Pad

/// Start Of Mouse
bool InputManager::mouse_button_pressed(MouseButtonCode button) {
    return m_mouseBindings[static_cast<size_t>(button)];
}

bool InputManager::mouse_button_held_down(MouseButtonCode button) {
    return m_mouseBindings[static_cast<size_t>(button)] && m_holdMouseBindings[static_cast<size_t>(button)];
}

bool InputManager::mouse_button_triggered(MouseButtonCode button) {
    return m_mouseBindings[static_cast<size_t>(button)] && !m_holdMouseBindings[static_cast<size_t>(button)];
}

bool InputManager::mouse_button_released(MouseButtonCode button) {
    return !m_mouseBindings[static_cast<size_t>(button)] && m_holdMouseBindings[static_cast<size_t>(button)];
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
    std::copy(std::begin(m_keyBinding), std::end(m_keyBinding), std::begin(m_holdKeyBindings));
    std::copy(std::begin(m_mouseBindings), std::end(m_mouseBindings), std::begin(m_holdMouseBindings));
    std::copy(std::begin(m_padBindings), std::end(m_padBindings), std::begin(m_holdPadBindings));
    m_lastScroll = vec2(0.0f);
}
}  // namespace knot
