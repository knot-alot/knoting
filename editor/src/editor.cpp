#include "editor.h"
#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/spot_light.h>

#include "demo_widget.h"
#include "widget_subsystem.h"

namespace knot {
Scene scene;
Editor::Editor() {
    Scene::set_active_scene(scene);
    log::Logger::setup();

    m_engine = std::make_unique<knot::Engine>();
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
        auto cubeObj = scene.create_game_object("loaded_dragon");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f - 5, 1.0f, -10.0f - 5));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 160, 0));
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
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f, 1.0f, -10.0f - 7));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 45, 0));
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
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f - 8, 1.0f, -10.0f + 2));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 90, 0));
        cubeObj.add_component<InstanceMesh>("dragon.obj");

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
        material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }

    auto widget_manager = std::make_shared<WidgetSubsystem>(m_engine);
    auto demo = std::make_shared<DemoWidget>("demo");

    widget_manager->add_widget(demo);

    m_engine->add_subsystem(std::make_shared<WidgetSubsystem>(m_engine));
}

void Editor::run() {
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
