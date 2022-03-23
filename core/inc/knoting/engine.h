#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <knoting/asset_manager.h>
#include <knoting/audio_subsystem.h>
#include <knoting/camera_rotation.h>
#include <knoting/forward_renderer.h>

#include <knoting/widget_subsystem.h>
#include <knoting/framebuffer_manager.h>
#include <knoting/networked_client.h>
#include <knoting/networked_server.h>

#include <knoting/physics.h>
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

    std::weak_ptr<NetworkedClient> get_client_module() { return m_clientModule; }
    std::weak_ptr<NetworkedServer> get_server_module() { return m_serverModule; }

    std::weak_ptr<FramebufferManager> get_framebuffer_manager_module() { return m_framebufferManager; }
    std::weak_ptr<AudioSubsystem> get_audio_module() { return m_audioModule; }
    std::weak_ptr<CameraRotation> get_camera_rotation_module() { return m_cameraRotationModule; }

    std::weak_ptr<WidgetSubsystem>get_Widget(){return m_WidgetSubsystem;}

    static std::optional<std::reference_wrapper<Engine>> get_active_engine();
    static void set_active_engine(std::optional<std::reference_wrapper<Engine>> engine);

    void add_subsystem(std::shared_ptr<Subsystem> subsystem);
    double get_bgfx_Time_cost(){
        return m_bgfxTimes;
    }
    double get_Phy_Time_cost(){
        return m_PhyTimes;
    }
    double get_Gui_Time_cost(){
        return m_GuiTimes;
    }

    inline static bool isClient = true;

   private:
    void swap_frame();
    void reset_physics_module();

   private:
    int m_windowWidth = 1024;
    int m_windowHeight = 768;
    std::string m_windowTitle = "hello knotting!";

    double m_bgfxTimes;
    double m_PhyTimes;
    double m_GuiTimes;
   private:


    std::vector<std::shared_ptr<Subsystem>> m_engineModules;

    std::shared_ptr<Window> m_windowModule;
    std::shared_ptr<ForwardRenderer> m_forwardRenderModule;
    std::shared_ptr<Physics> m_physicsModule;

    std::shared_ptr<NetworkedServer> m_serverModule;
    std::shared_ptr<NetworkedClient> m_clientModule;

    std::shared_ptr<FramebufferManager> m_framebufferManager;
    std::shared_ptr<AudioSubsystem> m_audioModule;
    std::shared_ptr<CameraRotation> m_cameraRotationModule;

    std::shared_ptr<WidgetSubsystem>m_WidgetSubsystem;

    inline static std::optional<std::reference_wrapper<Engine>> s_activeEngine = std::nullopt;
    std::shared_ptr<AssetManager> m_assetManager;
};

}  // namespace knot
