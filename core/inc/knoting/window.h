#pragma once

#include <inttypes.h>
#include <string>

class GLFWwindow;

namespace knot {

class Window {
   public:
    Window(int width, int height, std::string title);
    ~Window();

    bool is_open();
    void update();
    void draw();
    void close();

   protected:
    void setup_callbacks();

    static void window_size_callback(GLFWwindow* window, int width, int height);

    int m_width;
    int m_height;
    std::string m_title;

    GLFWwindow* m_window;
    std::uint16_t m_viewId;
};

}  // namespace knot
