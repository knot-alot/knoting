#include "untie.h"

#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/scene.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/scene.h>

#include <iostream>

namespace knot {
Scene scene;
Untie::Untie() {
    Scene::set_active_scene(scene);
    log::Logger::setup();

    m_engine = std::make_unique<knot::Engine>();
    {
        auto editorCamera = scene.create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(-10.0f, 15.0f, -30.0f));
    }

    {
        auto cubeObj = scene.create_game_object("cube_0");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0, -10, 0));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15.0f, 1.0f, 15.0f));

        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "b");
        material.set_texture_slot_path(TextureType::Roughness, "b2");
        material.set_texture_slot_path(TextureType::Occlusion, "b3");

        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("cube_1");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0.0f, 3.0f, 0.0f));

        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "b");
        material.set_texture_slot_path(TextureType::Roughness, "b2");
        material.set_texture_slot_path(TextureType::Occlusion, "b3");

        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("ground");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(1.0f, 7.0f, 1.0f));

        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "b");
        material.set_texture_slot_path(TextureType::Roughness, "b2");
        material.set_texture_slot_path(TextureType::Occlusion, "b3");

        cubeObj.add_component<components::Material>(material);
    }
}

void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
