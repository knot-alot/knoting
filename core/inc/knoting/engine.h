#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <knoting/forward_renderer.h>
#include <knoting/subsystem.h>
#include <knoting/window.h>
#include<knoting/physics.h>

namespace knot {
class Engine {
   public:
    Engine();
    ~Engine();

    void update_modules();
    bool is_open();

    std::weak_ptr<Window> get_window_module() { return m_windowModule; }
    std::weak_ptr<ForwardRenderer> get_forward_render_module() { return m_forwardRenderModule; }
    std::shared_ptr<Physics> get_physics_moddule() { return m_physicsModule; }

   private:
    int m_windowWidth = 1024;
    int m_windowHeight = 768;
    std::string m_windowTitle = "hello knotting!";

   private:
    std::vector<std::shared_ptr<Subsystem>> m_engineModules;
    std::shared_ptr<Window> m_windowModule;
    std::shared_ptr<ForwardRenderer> m_forwardRenderModule;
    std::shared_ptr<Physics> m_physicsModule;
};

}  // namespace knot
