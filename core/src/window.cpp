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
    : m_width(width),
      m_height(height),
      m_title(title),
      m_window(nullptr),
      m_engine(engine),
      m_windowResizedFlag(true),
      m_input(std::make_shared<InputManager>(*this)) {
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

    init.type = bgfx::RendererType::Vulkan;

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

void Window::window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (action != GLFW_REPEAT)
        self->m_input->key_event(key, action != GLFW_RELEASE);
}

void Window::window_char_callback(GLFWwindow* window, unsigned int) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
}

void Window::window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->m_input->mouse_button_event(button, action != GLFW_RELEASE);
}

void Window::window_scroll_event(GLFWwindow* window, double xoffset, double yoffset) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->m_input->scroll_event({xoffset, yoffset});
}

void Window::window_mouse_event_callback(GLFWwindow* window, double x, double y) {
    if (!glfwGetWindowAttrib(window, GLFW_HOVERED))
        return;
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->m_input->mouse_event(x, y);
}

void Window::window_cursor_enter_event_callback(GLFWwindow* window, int entered) {
    Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
}

void Window::recreate_framebuffer(int width, int height) {
    m_engine.get_framebuffer_manager_module().lock()->recreate_framebuffer(width, height);
}

void Window::setup_callbacks() {
    // TODO REPLACE ALL GLFW CALLBACKS WHEN IN EDITOR
    glfwSetWindowSizeCallback(m_window, Window::window_size_callback);

    glfwSetKeyCallback(m_window, Window::window_key_callback);
    glfwSetCharCallback(m_window, Window::window_char_callback);
    glfwSetMouseButtonCallback(m_window, Window::window_mouse_button_callback);
    glfwSetScrollCallback(m_window, Window::window_scroll_event);
    glfwSetCursorPosCallback(m_window, Window::window_mouse_event_callback);
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
    //    calculate_delta_time();
}

void Window::on_late_update() {
    m_lastTime = m_currentTime;
}

void Window::on_destroy() {}

void Window::calculate_delta_time() {
    m_currentTime = glfwGetTime();
    m_deltaTime = float(m_currentTime - m_lastTime);
}

double Window::get_delta_time() {
    return m_deltaTime;
}
void Window::set_window_size(vec2i size) {
    m_width = size.x;
    m_height = size.y;
    recreate_framebuffer(m_width, m_height);
}

std::shared_ptr<InputManager> Window::get_input_manager() {
    return m_input;
}

void Window::set_cursor_hide(bool state) {
    glfwSetInputMode(m_window, GLFW_CURSOR, state ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

}  // namespace knot
