#pragma once

#include <inttypes.h>
#include <string>

#include "knoting/subsystem.h"

class GLFWwindow;

namespace knot {

class Window : public Subsystem {
   public:
    Window(int width, int height, std::string title);
    ~Window();

    //=subsystem overrides=
    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;
    //=====================

    void calculate_delta_time();
    double get_delta_time();

    bool is_open();

    // TODO discuss refactor in code review
    void update();
    void draw();
    void close();
    // END TODO

   protected:
    void setup_callbacks();

    static void window_size_callback(GLFWwindow* window, int width, int height);

    int m_width;
    int m_height;
    std::string m_title;

    double m_deltaTime = 0;
    double m_fixedDeltaTime = 0.2;
    double m_currentFrame = 0;
    double m_lastFrame = 0;


    GLFWwindow* m_window;
    std::uint16_t m_viewId;
};

}  // namespace knot
