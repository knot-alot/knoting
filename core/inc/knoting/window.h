#pragma once

#include <string>

#include <inttypes.h>
#include <knoting/input_manager.h>
#include <knoting/subsystem.h>

namespace knot {

class Engine;

}

class GLFWwindow;

namespace knot {

class Window : public Subsystem {
   public:
    Window(int width, int height, std::string title, Engine& engine);
    ~Window();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void calculate_delta_time();
    double get_delta_time();

    bool is_open();
    void close();

    int get_window_width() { return m_width; };
    int get_window_height() { return m_height; };
    void set_window_size(vec2i size);
    vec2i get_window_size() { return vec2i(m_width, m_height); };

    float get_mouse_change_x() { return m_mouseWheelH; };
    float get_mouse_change_y() { return m_mouseWheel; };
    void set_mouse_change_x(float xOffset) { m_mouseWheelH = xOffset; };
    void set_mouse_change_y(float yOffset) { m_mouseWheel = yOffset; };
    void add_mouse_change_x(float xOffset) { m_mouseWheelH += xOffset; };
    void add_mouse_change_y(float yOffset) { m_mouseWheel += yOffset; };
    void set_cursor_hide(bool state);

    void recreate_framebuffer(int width, int height);

    GLFWwindow* get_glfw_window() { return m_window; };
    bool get_window_resize_flag() { return m_windowResizedFlag; };
    void set_window_resize_flag(bool newState) { m_windowResizedFlag = newState; };

    InputManager& get_input_manager();

   protected:
    bool m_windowResizedFlag;
    void setup_callbacks();
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_char_callback(GLFWwindow* window, unsigned int);
    static void window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void window_scroll_event(GLFWwindow* window, double xoffset, double yoffset);
    static void window_mouse_event_callback(GLFWwindow* window, double x, double y);
    static void window_cursor_enter_event_callback(GLFWwindow* window, int entered);

    int m_width;
    int m_height;
    float m_mouseWheelH = 0.0f;
    float m_mouseWheel = 0.0f;
    std::string m_title;

    double m_deltaTime = 0;
    double m_lastTime = 0;
    double m_currentTime = 0;

    static constexpr double PHYSICS_TIMESTEP = 0.2;
    double m_fixedDeltaTime = PHYSICS_TIMESTEP;

    GLFWwindow* m_window;
    std::uint16_t m_viewId;
    Engine& m_engine;
    InputManager m_input;
};

}  // namespace knot
