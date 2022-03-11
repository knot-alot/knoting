
#pragma once
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/sort.h>
#include <bx/string.h>
#include <bx/timer.h>

#include <fstream>
#include "knoting/font_manager.h"
#include "knoting/text_buffer_manager.h"

#include <wchar.h>

#include <bgfx/bgfx.h>
#include <knoting/components.h>
#include <knoting/engine.h>

namespace knot {
class Window;
}

namespace knot {
class Untie {
   public:
    Untie();

    GameObject create_skybox();
    GameObject create_pointlight(const std::string& name = "",
                                 vec3 position = vec3(),
                                 float innerRadius = 0,
                                 float outerRadius = 0,
                                 vec3 color = vec3(1));
    GameObject create_floor(const std::string& name = "", vec3 position = vec3(), float width = 0, float depth = 0);
    GameObject create_wall(const std::string& name = "",
                           vec3 position = vec3(0),
                           vec3 rotation = vec3(0),
                           vec3 scale = vec3(1));
    GameObject create_eg_wall(const std::string& name = "",
                              vec3 position = vec3(0),
                              vec3 rotation = vec3(0),
                              vec3 scale = vec3(1));
    GameObject create_cube(const std::string& name = "",
                           vec3 position = vec3(0),
                           vec3 rotation = vec3(0),
                           vec3 scale = vec3(1),
                           bool isDynamic = false,
                           float mass = 1);
    GameObject create_dragon(const std::string& name = "",
                             vec3 position = vec3(0),
                             vec3 rotation = vec3(0),
                             vec3 scale = vec3(1),
                             bool isDynamic = false,
                             float mass = 1);
    GameObject create_player(const std::string& name = "", vec3 position = vec3(0), vec3 rotation = vec3(0));
    void run();
    void serialize_test();

   private:
    std::unique_ptr<knot::Engine> m_engine;
    std::unique_ptr<knot::Scene> m_scene;
    std::unique_ptr<knot::Scene> m_loadedScene;
};

}  // namespace knot
