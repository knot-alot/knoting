#pragma once

#include <string>

#include <inttypes.h>
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

    int get_window_width(){return m_width;};
    int get_window_height(){return m_height;};

   protected:
    void setup_callbacks();
    static void window_size_callback(GLFWwindow* window, int width, int height);

    int m_width;
    int m_height;
    std::string m_title;

    double m_deltaTime = 0;
    double m_lastFrame = 0;

    static constexpr double PHYSICS_TIMESTEP = 0.2;
    double m_fixedDeltaTime = PHYSICS_TIMESTEP;

    GLFWwindow* m_window;
    std::uint16_t m_viewId;
    Engine& m_engine;
};

}  // namespace knot
