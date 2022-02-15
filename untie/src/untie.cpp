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
        //        auto cubeObj = scene.create_game_object("cube_0");
        //        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0, -10, 0));
        //        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15.0f, 1.0f, 15.0f));
        //
        //        auto& mesh = cubeObj.add_component<components::Mesh>();
        //        mesh.create_cube();
        //
        //        auto material = components::Material();
        //        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        //        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        //        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        //        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        //        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        //
        //        cubeObj.add_component<components::Material>(material);
    } {
        auto cubeObj = scene.create_game_object("loaded_cube");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f, 0.0f, -10.0f));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15, 1, 15));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 45, 0));

        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.load_mesh("uv_cube.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");

        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("stanford");
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(5));
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5, 2.0f, -10.0f));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 210, 0));

        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.load_mesh("dragon.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
        material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");

        cubeObj.add_component<components::Material>(material);
    }
    //    {
    //        auto cubeObj = scene.create_game_object("ground");
    //        cubeObj.get_component<components::Transform>().set_position(glm::vec3(1.0f, 7.0f, 1.0f));
    //
    //        auto& mesh = cubeObj.add_component<components::Mesh>();
    //        mesh.create_cube();
    //
    //        auto material = components::Material();
    //        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
    //        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
    //        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    //        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    //        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    //
    //        cubeObj.add_component<components::Material>(material);
    //    }
}

void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
