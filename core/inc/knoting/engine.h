#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <knoting/asset_manager.h>
#include <knoting/forward_renderer.h>
#include <knoting/framebuffer_manager.h>
#include <knoting/physics.h>
#include <knoting/player_movement_subsystem.h>
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
    std::weak_ptr<Physics> get_physics_module() { return m_physicsModule; }
    std::weak_ptr<FramebufferManager> get_framebuffer_manager_module() { return m_framebufferManager; }
    std::weak_ptr<PlayerMovement> get_player_movement_module() { return m_playerMovementModule; }

    static std::optional<std::reference_wrapper<Engine>> get_active_engine();
    static void set_active_engine(std::optional<std::reference_wrapper<Engine>> engine);

    void add_subsystem(std::shared_ptr<Subsystem> subsystem);

   private:
    void swap_frame();
    void reset_physics_module();

   private:
    int m_windowWidth = 1024;
    int m_windowHeight = 768;
    std::string m_windowTitle = "hello knotting!";

   private:
    std::vector<std::shared_ptr<Subsystem>> m_engineModules;

    std::shared_ptr<Window> m_windowModule;
    std::shared_ptr<ForwardRenderer> m_forwardRenderModule;
    std::shared_ptr<Physics> m_physicsModule;
    std::shared_ptr<FramebufferManager> m_framebufferManager;
    std::shared_ptr<PlayerMovement> m_playerMovementModule;

    inline static std::optional<std::reference_wrapper<Engine>> s_activeEngine = std::nullopt;
    std::shared_ptr<AssetManager> m_assetManager;
};

}  // namespace knot
