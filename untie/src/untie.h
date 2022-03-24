
#pragma once

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
    GameObject create_lower_floor(const std::string& name = "", vec3 position = vec3(), vec3 scale = vec3());
    GameObject create_upper_floor(const std::string& name = "", vec3 position = vec3(), vec3 scale = vec3());
    GameObject create_slim_lower_floor(const std::string& name = "", vec3 position = vec3(), vec3 scale = vec3());
    GameObject create_slim_upper_floor(const std::string& name = "", vec3 position = vec3(), vec3 scale = vec3());
    GameObject create_grass(const std::string& name = "", vec3 position = vec3(), vec3 scale = vec3());
    GameObject create_wall(const std::string& name = "", vec3 position = vec3(0), vec3 rotation = vec3(0),
                              vec3 scale = vec3(1));
    GameObject create_brick_wall(const std::string& name = "", vec3 position = vec3(0), vec3 rotation = vec3(0),
                           vec3 scale = vec3(1));
    GameObject create_ramp(const std::string& name = "", vec3 position = vec3(), vec3 rotation = vec3(), vec3 scale = vec3());

    GameObject create_player(const std::string& name = "", vec3 position = vec3(), vec3 rotation = vec3(), int playerNum = 0);

    void create_level();

    void run();
    void serializeTest();
   private:
    std::unique_ptr<knot::Engine> m_engine;
    std::unique_ptr<knot::Scene> m_scene;
};

}  // namespace knot
