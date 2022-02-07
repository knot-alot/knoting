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
    auto editorCamera = scene.create_game_object("camera");
    auto& cam = editorCamera.add_component<components::EditorCamera>();


    auto cubeObj = scene.create_game_object("cube");
    auto& mesh = cubeObj.add_component<components::Mesh>();
    mesh.create_cube();
    auto material = cubeObj.add_component<components::Material>();

}

void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
