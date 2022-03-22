#include "untie.h"
#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/post_processing.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/scene.h>
#include <knoting/texture.h>

#include <knoting/audio_listener.h>
#include <knoting/audio_source.h>
#include <knoting/audio_subsystem.h>
#include <knoting/components.h>
#include <knoting/scene.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <iostream>

#include <cereal/archives/json.hpp>

#include <iostream>

namespace knot {

std::optional<GameObject> cubeOne;
std::optional<GameObject> cubeTwo;

Untie::Untie() {
    m_scene = std::make_unique<Scene>();
    Scene::set_active_scene(*m_scene);
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();
    Engine::set_active_engine(*m_engine);
    create_level();
}

void Untie::run() {
    auto cliMod = m_engine->get_client_module().lock();
    cliMod->attempt_connection();
    while (m_engine->is_open()) {
        m_engine->update_modules();
        auto im = m_engine->get_window_module().lock()->get_input_manager();

        if (im->key_on_trigger(KeyCode::P)) {
            if (cubeOne) {
                components::AudioSource& source = cubeOne->get_component<components::AudioSource>();
                auto weakAudioModule = m_engine->get_audio_module();
                if (!weakAudioModule.expired()) {
                    auto audioModule = weakAudioModule.lock();
                    audioModule->toggle(source);
                }
            }
        }

        if (im->key_on_trigger(KeyCode::Y)) {
            if (cubeOne) {
                components::AudioSource& source = cubeTwo->get_component<components::AudioSource>();
                auto weakAudioModule = m_engine->get_audio_module();
                if (!weakAudioModule.expired()) {
                    auto audioModule = weakAudioModule.lock();
                    audioModule->play(source);
                }
            }
        }

        if (im->key_pressed(KeyCode::Escape)) {
            m_engine->get_window_module().lock()->close();
        }
    }
}
GameObject Untie::create_floor(const std::string& name, vec3 position, float width, float depth) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(width, 0.5f, depth));
    cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 0, 0));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(vec3(width, 0.5f, depth));
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.add_aggregate("level", 100, false);

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(false);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
    material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    return cubeObj;
}
GameObject Untie::create_wall(const std::string& name, vec3 position, vec3 rotation, vec3 scale) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(scale);
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("EG_wall_main.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.find_aggregate("level");

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
GameObject Untie::create_player(const std::string& name, vec3 position, vec3 rotation, int playerNum)    {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("dragon.obj");

    auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(1.5f,1.f,0.5f);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.add_aggregate(name, 5, false);

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(true, 4.0f);

    auto& rigidController = cubeObj.add_component<components::RigidController>();
    rigidController.lockRotations(true, false, true);
    rigidController.set_linear_damping(1.0f);
    rigidController.set_angular_damping(1.0f);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
    material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    auto& client = cubeObj.add_component<components::ClientPlayer>();
    client.m_clientNum = playerNum;

    return cubeObj;
}
void Untie::serializeTest() {
//        std::string filename("post_process.json");
//        std::filesystem::path path = AssetManager::get_resources_path().append(filename);
//        std::fstream serializedSceneStream(path);
//
//        serializedSceneStream.open(path, std::ios_base::out);
//        scene.save_scene_to_stream(serializedSceneStream);
//        serializedSceneStream.close();
//        Scene::set_active_scene(loadedScene);
//        serializedSceneStream.open(path, std::ios_base::in);
//        if (serializedSceneStream) {
//            loadedScene.load_scene_from_stream(serializedSceneStream);
//        } else {
//            log::debug("file not found");
//        }
//        serializedSceneStream.close();
}
GameObject Untie::create_point_light(const std::string& name,vec3 position,float innerRadius,float outerRadius, vec3 color)
{
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
    cubeObj.get_component<components::Transform>().set_position(glm::vec3(0, 8, 0));
    cubeObj.add_component<components::InstanceMesh>("postProcessPlane");

    auto skybox = components::SkyBox();

    skybox.set_texture_slot_path(SkyBoxTextureType::SkyBox, "skybox/cmft_skybox.hdr");
    skybox.set_texture_slot_path(SkyBoxTextureType::Irradiance, "skybox/cmtr_irradiance.hdr");
    skybox.set_texture_slot_path(SkyBoxTextureType::Radiance, "skybox/cmtr_radiance.hdr");
    skybox.set_uniform_background_type(SkyBoxTextureType::Radiance);

    cubeObj.add_component<components::SkyBox>(skybox);

    return cubeObj;
}
GameObject Untie::create_post_processing() {
    auto cubeObj = m_scene->create_game_object("post processing");
    cubeObj.get_component<components::Transform>().set_position(vec3(0));
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(7, 7, 7));
    cubeObj.add_component<components::InstanceMesh>("postProcessPlane");
    cubeObj.add_component<components::PostProcessing>();

    return cubeObj;
}
void Untie::create_level() {
    {
        auto editorCamera = m_scene->create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(-0.0f, 50.0f, 0.0f));
        editorCamera.add_component<components::AudioListener>();
    }

    create_skybox();

    create_point_light("light_0",vec3(0,10,0),30,0.5f,vec3(1,0.7f,0.2f));

    create_floor("floor", vec3(0,0,0),40,60);

    create_wall("edge_wall", vec3(39.52f,1.12f,10.88f), vec3(0,90,0), vec3(12,2,1));

    create_player("player_1",vec3(-20,5,-20),vec3(0),0);
    create_player("player_2",vec3(20,5,-20),vec3(0),1);
    create_player("player_3",vec3(-20,5,20),vec3(0),2);
    create_player("player_4",vec3(20,5,20),vec3(0),3);
    create_player("player_5",vec3(0,5,-20),vec3(0),4);
    create_player("player_6",vec3(0,5,20),vec3(0),5);

    create_post_processing();
}

}  // namespace knot
