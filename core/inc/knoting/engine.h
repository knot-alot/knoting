#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <knoting/asset_manager.h>
#include <knoting/forward_renderer.h>
#include <knoting/subsystem.h>
#include <knoting/window.h>

namespace knot {
class Engine {
   public:
    Engine();
    ~Engine();

    void update_modules();
    bool is_open();

    std::weak_ptr<Window> get_window_module() { return m_windowModule; }
    std::weak_ptr<ForwardRenderer> get_forward_render_module() { return m_forwardRenderModule; }
    void add_subsystem(std::shared_ptr<Subsystem> subsystem);
   private:
    void swap_frame();

   private:
    int m_windowWidth = 1024;
    int m_windowHeight = 768;
    std::string m_windowTitle = "hello knotting!";

   private:
    std::vector<std::shared_ptr<Subsystem>> m_engineModules;
    std::shared_ptr<Window> m_windowModule;
    std::shared_ptr<ForwardRenderer> m_forwardRenderModule;
    std::shared_ptr<AssetManager> m_assetManager;
};

}  // namespace knot
