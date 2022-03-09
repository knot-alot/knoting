#include <knoting/assert.h>
#include <knoting/engine.h>
#include <knoting/log.h>
#include <knoting/window.h>

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/bx.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace knot {

Window::Window(int width, int height, std::string title, Engine& engine)
    : m_width(width), m_height(height), m_title(title), m_window(nullptr), m_engine(engine), m_windowResizedFlag(true) {
    int glfw_init_res = glfwInit();

    KNOTING_ASSERT_MESSAGE(glfw_init_res == GLFW_TRUE, "Failed to initialize GLFW");
    log::debug("GLFW initialized");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    KNOTING_ASSERT_MESSAGE(m_window != nullptr, "Failed to create window");
    log::debug("Window created, title \"{}\", dimensions {}x{}", m_title, m_width, m_height);

    glfwSetWindowUserPointer(m_window, this);
    setup_callbacks();

    // To avoid creating a render thread we need to call renderFrame() manually
    bgfx::renderFrame();
    bgfx::Init init;

#if BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(m_window);
#elif BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(std::uintptr_t)glfwGetX11Window(m_window);
#endif

    int bgfx_init_res = bgfx::init(init);

    KNOTING_ASSERT_MESSAGE(bgfx_init_res != 0, "Failed to initialize bgfx");
}

Window::~Window() {
    bgfx::shutdown();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    log::debug("Window destroyed");
}

void Window::window_size_callback(GLFWwindow* window, int width, int height) {
    Window* self = (Window*)glfwGetWindowUserPointer(window);
    self->m_width = width;
    self->m_height = height;
    self->recreate_framebuffer(width, height);
    // TODO REPLACE THIS FUNCTION & ALL GLFW CALLBACKS WHEN IN EDITOR
    self->set_window_resize_flag(true);
}
void Window::recreate_framebuffer(int width, int height) {
    m_engine.get_framebuffer_manager_module().lock()->recreate_framebuffer(width, height);
}

void Window::setup_callbacks() {
    // TODO REPLACE ALL GLFW CALLBACKS WHEN IN EDITOR
    glfwSetWindowSizeCallback(m_window, Window::window_size_callback);
}

bool Window::is_open() {
    return !glfwWindowShouldClose(m_window);
}

void Window::close() {
    log::warn("closing glfw window");
    glfwSetWindowShouldClose(m_window, true);
}

void Window::on_awake() {}

void Window::on_update(double m_delta_time) {
    glfwPollEvents();
    calculate_delta_time();
}

void Window::on_late_update() {}

void Window::on_destroy() {}

void Window::calculate_delta_time() {
    double currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
}

double Window::get_delta_time() {
    return m_deltaTime;
}
void Window::set_window_size(vec2i size) {
    m_width = size.x;
    m_height = size.y;
    recreate_framebuffer(m_width, m_height);
}

}  // namespace knot
