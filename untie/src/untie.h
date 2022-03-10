
#pragma once

#include <knoting/engine.h>

namespace knot {
class Window;
}

namespace knot {
class Untie {
   public:
    Untie();

    GameObject* create_skybox();
    GameObject* create_pointlight(std::string name, vec3 position, float innerRadius, float outerRadius, vec3 color);
    GameObject* create_floor(std::string name, vec3 position, float width, float depth);
    GameObject* create_wall(std::string name, vec3 position = vec3(0), vec3 rotation = vec3(0), vec3 scale = vec3(1));
    GameObject* create_cube(std::string name,
                            vec3 position = vec3(0),
                            vec3 rotation = vec3(0),
                            vec3 scale = vec3(1),
                            bool isDynamic = false,
                            float mass = 1);
    GameObject* create_dragon(std::string name,
                              vec3 position = vec3(0),
                              vec3 rotation = vec3(0),
                              vec3 scale = vec3(1),
                              bool isDynamic = false,
                              float mass = 1);
    GameObject* create_player(std::string name, vec3 position = vec3(0), vec3 rotation = vec3(0));
    void run();
    void serialize_test();

   private:
    std::unique_ptr<knot::Engine> m_engine;
    std::unique_ptr<knot::Scene> m_scene;
    std::unique_ptr<knot::Scene> m_loadedScene;
};

}  // namespace knot
