#include "untie.h"
#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/scene.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/scene.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <iostream>

#include <cereal/archives/json.hpp>
#include <iostream>

namespace knot {
Untie::Untie() {
    m_scene = std::make_unique<Scene>();
    m_loadedScene = std::make_unique<Scene>();
    Scene::set_active_scene(*m_scene);
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();
    Engine::set_active_engine(*m_engine);

    {
        auto editorCamera = m_scene->create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(0, 150, 0));
        //editorCamera.get_component<components::Transform>().set_rotation_euler(glm::vec3(90, 00, 0));
    }

    create_skybox();
    create_pointlight("main_light", vec3(0, 40, 0), 0.5f, 75.0f, vec3(1.0f, 0.95f, 0.72f));
    create_pointlight("blue_light_1", vec3(35, 8, 35), 0.5f, 25.0f, vec3(0.0f, 0.0f, 1.0f));
    create_pointlight("blue_light_2", vec3(35, 8, -35), 0.5f, 25.0f, vec3(0.0f, 0.0f, 1.0f));
    create_pointlight("orange_light_top", vec3(-35, 8, -35), 0.5f, 25.0f, vec3(1.0f, 0.27f, 0.0f));
    create_pointlight("orange_light_bottom", vec3(-35, 8, 35), 0.5f, 25.0f, vec3(1.0f, 0.27f, 0.0f));

    //create_pointlight("light_3", vec3(7.6706734, 3.631392, -10), 0.5f, 17.0f, vec3(1.0f, 0.4f, 0.2f));

    create_floor("floor", vec3(0.0f, 0.0f, 0.0f), 50, 50);
    //create_wall("wall_1", vec3(0, 5, 0), vec3(0, 0, 0), vec3(15.0f, 5, 1));
    //create_wall("wall_top", vec3(-5, 5, 25), vec3(0, 0, 0), vec3(15.0f, 5, 1));
    //create_wall("wall_left", vec3(-5, 5, 25), vec3(0, 0, 0), vec3(15.0f, 5, 1));

    create_eg_wall("evil_genius_wall_top", vec3(0, 12, 50), vec3(0, 0, 180), vec3(100, 4, 1));
    create_eg_wall("evil_genius_wall_bottom", vec3(0, 12, -50), vec3(0, 0, -180), vec3(100, 4, 1));
    create_eg_wall("evil_genius_wall_right", vec3(50, 12, 0), vec3(180, -90, 0), vec3(100, 4, 1));
    create_eg_wall("evil_genius_wall_left", vec3(-50, 12, 0), vec3(-180, 90, 0), vec3(100, 4, 1));

    create_eg_wall("evil_genius_wall_middle_right", vec3(20, 12, -20), vec3(0, -90, 180), vec3(60, 4, 1));
    create_eg_wall("evil_genius_wall_middle_left", vec3(-20, 12, 20), vec3(0, -90, 180), vec3(60, 4, 1));

    //m_scene->create_cube("cube_1", vec3(-35, 1, 35), vec3(0, 0, 0), vec3(2, 2, 2), false, 5);
    //create_dragon("dragon_1", vec3(-8, 10, -5), vec3(45, 45, 45), vec3(5, 5, 5), true, 4);

    //create_player("player_1", vec3(-10, 6, -2), vec3(0, 33, 0));
}
void Untie::run() {
    while (m_engine->is_open()) {
        m_engine->update_modules();
        auto im = m_engine->get_window_module().lock()->get_input_manager();
        if (im->key_pressed(KeyCode::Escape)) {
            m_engine->get_window_module().lock()->close();
        }
    }
}

GameObject Untie::create_pointlight(const std::string& name,
                                    vec3 position,
                                    float innerRadius,
                                    float outerRadius,
                                    vec3 color) {
    auto light = m_scene->create_game_object(name);
    auto& spotLight = light.add_component<components::SpotLight>();
    spotLight.set_color(color);
    spotLight.set_outer_radius(outerRadius);
    spotLight.set_inner_radius(innerRadius);
    light.get_component<components::Transform>().set_position(position);

    return light;
}

GameObject Untie::create_skybox() {
    auto cubeObj = m_scene->create_game_object("skybox");
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(30, 30, 30));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto skybox = components::SkyBox();
    skybox.set_texture_slot_path(SkyBoxTextureType::SkyBox, "skybox/cmft_skybox.hdr");
    skybox.set_texture_slot_path(SkyBoxTextureType::Irradiance, "skybox/cmtr_irradiance.hdr");
    skybox.set_texture_slot_path(SkyBoxTextureType::Radiance, "skybox/cmtr_radiance.hdr");
    cubeObj.add_component<components::SkyBox>(skybox);

    return cubeObj;
}
GameObject Untie::create_floor(const std::string& name, vec3 position, float width, float depth) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(width, 0.5f, depth));
    cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 0, 0));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(width, 0.5f, depth);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
    material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    rigidbody.create_actor(false);

    return cubeObj;
}
GameObject Untie::create_wall(const std::string& name, vec3 position, vec3 rotation, vec3 scale) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.get_component<components::Transform>().set_scale(scale);
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");
    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(false);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "wall_tex.png");
    material.set_texture_slot_path(TextureType::Normal, "wall_normal.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    return cubeObj;
}

GameObject Untie::create_eg_wall(const std::string& name, vec3 position, vec3 rotation, vec3 scale) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.get_component<components::Transform>().set_scale(scale);
    cubeObj.add_component<components::InstanceMesh>("EG_wall_main.obj");
    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(false);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "wall_tex.png");
    material.set_texture_slot_path(TextureType::Normal, "wall_normal.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    return cubeObj;
}

GameObject Untie::create_cube(const std::string& name,
                              vec3 position,
                              vec3 rotation,
                              vec3 scale,
                              bool isDynamic,
                              float mass) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(scale);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");
    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(isDynamic, mass);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "orange.png");
    material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);
    return cubeObj;
}
GameObject Untie::create_dragon(const std::string& name,
                                vec3 position,
                                vec3 rotation,
                                vec3 scale,
                                bool isDynamic,
                                float mass) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(scale);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("dragon.obj");

    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale / 2.0f);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(isDynamic, mass);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
    material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    return cubeObj;
}

GameObject Untie::create_player(const std::string& name, vec3 position, vec3 rotation) {
    auto cubeObj = m_scene->create_game_object(name);

    cubeObj.add_component<components::Tag>();
    cubeObj.get_component<components::Tag>().register_tag("PLAYER");

    vec3 playerScale = vec3(1, 2, 1);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(playerScale);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(playerScale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(true, 8);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
    material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    // TODO: ADD CAMERA AS CHILD OBJECT AT TOP OF PLAYER

    return cubeObj;
}

void Untie::serialize_test() {
    std::string filename("skyboxScene.json");
    std::filesystem::path path = AssetManager::get_resources_path().append(filename);
    std::fstream serializedSceneStream(path);

    serializedSceneStream.open(path, std::ios_base::out);
    m_scene->save_scene_to_stream(serializedSceneStream);
    serializedSceneStream.close();
    Scene::set_active_scene(*m_loadedScene);
    serializedSceneStream.open(path, std::ios_base::in);
    if (serializedSceneStream) {
        m_loadedScene->load_scene_from_stream(serializedSceneStream);
    } else {
        log::debug("file not found");
    }
    serializedSceneStream.close();
}

}  // namespace knot
