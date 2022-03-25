
#pragma once

#include <knoting/Debug_gui.h>
#include "knoting/demo_widget.h"
#include <knoting/win_lose_widget.h>
#include <knoting/Menu.h>
#include <knoting/PauseMenu.h>
#include <knoting/debug_physics_widget.h>
#include <knoting/engine.h>
namespace knot {
class Window;
}

namespace knot {
class Untie {
   public:
    Untie();

    GameObject create_point_light(const std::string& name,
                                  vec3 position,
                                  float innerRadius,
                                  float outerRadius,
                                  vec3 color);
    GameObject create_skybox();
    GameObject create_post_processing();

    GameObject create_level_bottom();
    GameObject create_paint_tank_base(const std::string& name = "",
                                      vec3 position = vec3(1),
                                      vec3 rotation = vec3(0),
                                      vec3 scale = vec3(1));

    GameObject create_paint_tank_glass_red(const std::string& name = "",
                                           vec3 position = vec3(1),
                                           vec3 rotation = vec3(0),
                                           vec3 scale = vec3(1));

    GameObject create_paint_tank_glass_blue(const std::string& name = "",
                                            vec3 position = vec3(1),
                                            vec3 rotation = vec3(0),
                                            vec3 scale = vec3(1));

    GameObject create_master_floor(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_lower_floor(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_upper_floor(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_slim_lower_floor(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_slim_upper_floor(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_grass(const std::string& name = "", vec3 position = vec3(0), vec3 scale = vec3(1));
    GameObject create_wall(const std::string& name = "",
                           vec3 position = vec3(0),
                           vec3 rotation = vec3(0),
                           vec3 scale = vec3(1));
    GameObject create_brick_wall(const std::string& name = "",
                                 vec3 position = vec3(0),
                                 vec3 rotation = vec3(0),
                                 vec3 scale = vec3(1));
    GameObject create_ramp(const std::string& name = "",
                           vec3 position = vec3(0),
                           vec3 rotation = vec3(0),
                           vec3 scale = vec3());

    GameObject create_player(const std::string& name = "",
                             vec3 position = vec3(0),
                             vec3 rotation = vec3(0),
                             int playerNum = 0);

    GameObject create_spawn_location(const std::string& name = "", vec3 position= vec3(0), vec3 rotation = vec3(0), vec3 scale = vec3(1));

    GameObject create_death_location(const std::string& name = "", vec3 position= vec3(0), vec3 rotation = vec3(0), vec3 scale = vec3(1));
    GameObject create_audio_hub();

    void create_level();

    void run();
    void serializeTest();

   private:
    bool open = true;
    std::shared_ptr<knot::PauseMenu> m_Pause_menu;
    std::shared_ptr<knot::Debug_gui> m_debug;
    std::shared_ptr<knot::DebugPhysics> m_debugPhysics;
    std::shared_ptr<knot::DemoWidget> m_demoWidget;
    std::shared_ptr<knot::WinLoseWidget> m_winLoseWidget;
    std::shared_ptr<knot::Menu> m_menu;
    std::unique_ptr<knot::Engine> m_engine;
    std::unique_ptr<knot::Scene> m_scene;
};

}  // namespace knot
