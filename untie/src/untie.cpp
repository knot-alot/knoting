#include "untie.h"

#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/scene.h>
#include <knoting/texture.h>

#include <iostream>

namespace knot {

Untie::Untie() {
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();

    Scene scene;
    Scene::set_active_scene(scene);
    auto cubeObj = scene.create_game_object("cube");
}

void Untie::run() {
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
