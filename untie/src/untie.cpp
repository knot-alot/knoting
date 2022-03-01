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
#include <knoting/spot_light.h>

#include <iostream>

namespace knot {
Scene scene;
Untie::Untie() {
    Scene::set_active_scene(scene);
    log::Logger::setup();

    m_engine = std::make_unique<knot::Engine>();
    Engine::set_active_engine(*m_engine);
    {
        auto editorCamera = scene.create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(-10.0f, 15.0f, -30.0f));
    }
    {
        auto light = scene.create_game_object("light_0");
        auto& spotLight = light.add_component<components::SpotLight>();
        spotLight.set_color(vec3(1.0f, 0.7f, 0.2f));
        spotLight.set_outer_radius(17.0f);
        spotLight.set_inner_radius(0.5f);
        light.get_component<components::Transform>().set_position(glm::vec3(2.3897731, 14.570069, -10));
    }
    {
        auto light = scene.create_game_object("light_1");
        auto& spotLight = light.add_component<components::SpotLight>();
        spotLight.set_color(vec3(0.7f, 0.2f, 1.0f));
        spotLight.set_outer_radius(17.0f);
        spotLight.set_inner_radius(0.5f);
        light.get_component<components::Transform>().set_position(glm::vec3(10.351221, -13.538474, -10));
    }
    {
        auto light = scene.create_game_object("light_2");
        auto& spotLight = light.add_component<components::SpotLight>();
        spotLight.set_color(vec3(0.2f, 1.0f, 0.7f));
        spotLight.set_outer_radius(17.0f);
        spotLight.set_inner_radius(0.5f);
        light.get_component<components::Transform>().set_position(glm::vec3(-14.905335, 6.3970194, -10));
    }
    {
        auto light = scene.create_game_object("light_3");
        auto& spotLight = light.add_component<components::SpotLight>();
        spotLight.set_color(vec3(1.0f, 0.4f, 0.2f));
        spotLight.set_outer_radius(17.0f);
        spotLight.set_inner_radius(0.5f);
        light.get_component<components::Transform>().set_position(glm::vec3(7.6706734, 3.631392, -10));
    }
    {
        auto cubeObj = scene.create_game_object("loaded_cube");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f, 0.0f, -10.0f));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15, 1, 15));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 45, 0));
        cubeObj.add_component<InstanceMesh>("uv_cube.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("loaded_dragon");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f, 1.0f, -10.0f));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(5, 5, 5));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 180, 0));
        cubeObj.add_component<InstanceMesh>("dragon.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
        material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("loaded_dragon");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f + 5, 1.0f, -10.0f - 5));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 240, 0));
        cubeObj.add_component<InstanceMesh>("dragon.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
        material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("ground");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0, -10, 0));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15.0f, 1.0f, 15.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();
        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(15.0, 2.0f, 15.0);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(false);
    }
    {
        auto cubeObj = scene.create_game_object("cube_1");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0.0f, 3.0f, 0.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f, 1.0f, 1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(true, 5.0f);
    }
    {
        auto cubeObj = scene.create_game_object("cube_0");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(1.0f, 7.0f, 1.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();
        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f, 1.0f, 1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));
        auto& rigidbody = cubeObj.add_component<components::RigidBody>();
        rigidbody.create_actor(true, 5.0f);
    }
}
void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
