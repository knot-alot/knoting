#include "untie.h"

#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/scene.h>

#include <knoting/components.h>
#include <knoting/scene.h>

#include <iostream>

namespace knot {
    Scene scene;
Untie::Untie() {
    Scene::set_active_scene(scene);
    log::Logger::setup();

    m_engine = std::make_unique<knot::Engine>();
    auto cubeObj = scene.create_game_object("cube");
    auto& mesh = cubeObj.add_component<components::Mesh>();
    mesh.create_cube();
    auto& tex = cubeObj.add_component<components::Texture>();
//    tex.load_texture_2d()
}

void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
