#pragma once
#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <bgfx/bgfx.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <knoting/engine.h>
#include <memory>
#include <vector>
#include "knoting/log.h"
#include "knoting/subsystem.h"
#include "widget.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "editor_precompiled_shader.h"

static bgfx::VertexLayout imguiVertexLayout;
static bgfx::TextureHandle imguiFontTexture;
static bgfx::UniformHandle imguiFontUniform;
static bgfx::ProgramHandle imguiProgram;

namespace knot {

class WidgetSubsystem : public Subsystem {
   public:
    explicit WidgetSubsystem(std::weak_ptr<knot::Engine> engine);
    void add_widget(std::shared_ptr<Widget> widget);

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

   private:
    void imgui_init(GLFWwindow* window);

    void imgui_render(ImDrawData* drawData);
    void imgui_shutdown();
    void imgui_reset(uint16_t width, uint16_t height);
    void imgui_poll_events(float dt);

    // USING DEFINE FROM editor_precompiled_shaders.h
    // TODO USING PRECOMPILED SHADERS AS LOADING FROM FILE IS WORKING INCORRECTLY
    // TODO FIX SHADER LOADING INCORRECTLY PROBABLY SHADER C WORKING INCORRECTLY
    _getShader(vs_ocornut_imgui);
    _getShader(fs_ocornut_imgui);

   private:
    std::vector<std::shared_ptr<Widget>> m_widgets;
    std::weak_ptr<knot::Engine> m_engine;
    components::ShaderProgram shader_test;
    double m_deltaTime = 0.01;

   public:
    // TODO should be managed inside of Input manager instead + extend functionality in editor
    void set_glfw_editor_callbacks(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static void charModsCallback(GLFWwindow* window, unsigned int codepoint, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void cursorEnterCallback(GLFWwindow* window, int entered);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void dropCallback(GLFWwindow* window, int count, const char** paths);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);
};

}  // namespace knot
