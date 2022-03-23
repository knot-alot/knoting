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
GameObject Untie::create_floor(const std::string& name, vec3 position, vec3 scale) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(scale));
    cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 0, 0));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(vec3(scale));
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.find_aggregate("level");

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

GameObject Untie::create_grass(const std::string& name, vec3 position, vec3(scale)) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(scale));
    cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 0, 0));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(vec3(scale));
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.find_aggregate("level");

    auto& rigidbody = cubeObj.add_component<components::RigidBody>();

    rigidbody.create_actor(false);

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "grass1-albedo3.png");
    material.set_texture_slot_path(TextureType::Normal, "grass1-normal1-ogl.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    cubeObj.add_component<components::Material>(material);

    return cubeObj;
}

GameObject Untie::create_ramp(const std::string& name, vec3 position, vec3 rotation, vec3 scale) {
    auto cubeObj = m_scene->create_game_object(name);
    cubeObj.get_component<components::Transform>().set_position(position);
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(scale));
    cubeObj.get_component<components::Transform>().set_rotation_euler(rotation);
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(scale);
    shape.set_geometry(shape.create_cube_geometry(halfsize));

    auto& aggregate = cubeObj.add_component<components::Aggregate>();
    aggregate.find_aggregate("level");

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
    vec3 halfsize = vec3(scale) * vec3(0.5,0.5,0.3);
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
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(1, 1, 1));
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

    create_level_bottom();
    create_floor("lower_floor_top", vec3(0.229999542,0,-31),vec3(30,1,18.7826252));
    create_floor("lower_floor_bottom", vec3(0.229999542,0,34.5),vec3(30,1,16.14291));
    create_floor("lower_floor_left", vec3(25.25,0,3), vec3(4.57548952,1,15.436904));
    create_floor("lower_floor_right", vec3(-25,0,3), vec3(4.37945986,1,15.436904));
    create_floor("upper_floor_top", vec3(-0.100001574,7,-15.5599995), vec3(8.5098753,0.100000001,3.42172575));
    create_floor("upper_floor_bottom", vec3(-0.100001574,7,20.9400024), vec3(8.5098753,0.100000001,3.85423183));
    create_floor("upper_floor_right", vec3(-14.460001,7,2.89000058), vec3(6.40984821,0.091787003,21.8651066));
    create_floor("upper_floor_left", vec3(14.5400028,7,2.89000058), vec3(6.40984821,0.091787003,21.8651066));
    create_floor("floor_cover", vec3(17.5,7,32.5), vec3(3.30587935,0.100000001,7.7004447));

    create_grass("Lower_floor_grass", vec3(0.229999542,0,3.25), vec3(20.5185394,1.05, 15.5414333));

    create_ramp("Bottom_ramp", vec3(11.25,4.01999998,32.5),vec3(21.4499741,0,0), vec3(2.80200028,0.0252700001,8.42736721));
    create_ramp("Top_ramp", vec3(-10.5,4,-26.5),vec3(338.550018,0,0), vec3(3.10413098,0.128035069,8.33843422));

    create_wall("edge_wall_left1", vec3(29.6299992,1.12,10.8999996), vec3(0,90,0), vec3(16,2,1));
    create_wall("edge_wall_left2", vec3(29.6200008,1.12,26.8400002), vec3(0,90,0), vec3(16,2,1));
    create_wall("edge_wall_left3", vec3(29.6299992,1.12,42.2400017), vec3(0,90,0), vec3(15.58,2,1));
    create_wall("edge_wall_left4", vec3(29.6299992,1.12,-5.11000013), vec3(0,90,0), vec3(16,2,1));
    create_wall("edge_wall_left5", vec3(29.6299992,1.12,-21.0799999), vec3(0,90,0), vec3(16,2,1));
    create_wall("edge_wall_left6", vec3(29.6299992,1.12,-37.0600014), vec3(0,90,0), vec3(16,2,1));
    create_wall("edge_wall_left7", vec3(29.6299992,1.12,-47.3600006), vec3(0,90,0), vec3(4.73758793,2,1));

    create_wall("edge_wall_right1", vec3(-29.5876465,1.11999893,41.9527817), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right2", vec3(-29.5876465,1.11999893,26.0027847), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right3", vec3(-29.5876465,1.11999893,10.0027847), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right4", vec3(-29.5876465,1.11999893,-5.95721436), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right5", vec3(-29.5876465,1.11999893,-21.9172153), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right6", vec3(-29.5876465,1.11999893,-37.8372154), vec3(0,-90,0), vec3(16,2,1));
    create_wall("edge_wall_right7", vec3(-29.5876465,1.11999893,-47.7672157), vec3(0,-90,0), vec3(4.5112009,2,1));

    create_wall("low_cover_right", vec3(27.8199997,1.12,0.0500000007), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_right_2", vec3(27.8200035,1.07000005,-0.287000179), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_right2", vec3(23.0400028,1.07000005,-14.9869995), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_right2_2", vec3(23.039999,1.12,-14.6499996), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_right3", vec3(22.9399986,1.07000005,22.3700008), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_right3_2", vec3(22.9399948,1.12,22.7070007), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_top", vec3(14.1199999,1.07000005,-15.46), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_top_2", vec3(14.0999994,1.12,-15.1230001), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left", vec3(-27.3299942,1.07000005,-31.046999), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left_2", vec3(-27.3299999,1.12,-30.7099991), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left2", vec3(-23,1.07000005,-19.507), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left2_2", vec3(-23.0000057,1.12,-19.1700001), vec3(0,180,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left3", vec3(-27.25,1.07000005,4.11000013), vec3(0,0,0), vec3(4.27674007,0.5,1));
    create_wall("low_cover_left3_2", vec3(-27.2500057,1.12,4.44699955), vec3(0,180,0), vec3(4.27674007,0.5,1));

    create_wall("Top_base_wall_small", vec3(11.8499994,1.12,-37.9500008), vec3(0,-90,0), vec3(8.15449715,2,1));
    create_wall("Top_base_wall_small_2", vec3(11.54,1.12,-37.9000015), vec3(0,90,0), vec3(8.15449715,2,1));
    create_wall("Top_base_back", vec3(2,1.12,-41.5800018), vec3(0,180,0), vec3(19.428257,2,1));
    create_wall("Top_base_back_2", vec3(1.97999859,1.12,-41.9500008), vec3(0,0,0), vec3(19.428257,2,1));
    create_wall("Top_base_front", vec3(15.6499996,1.12,-29.6299992), vec3(0,0,0), vec3(28.8810825,2,1));
    create_wall("Top_base_front_2", vec3(15.6700001,1.12,-29.2600002), vec3(0,180,0), vec3(28.8810825,2,1));
    create_wall("Small_top_base", vec3(6.55999947,1.12,-24), vec3(0,0,0), vec3(10.519865,2,1));
    create_wall("Small_top_base_2", vec3(6.55999947,1.12,-23.6800003), vec3(0,180,0), vec3(10.519865,2,1));
    create_wall("Small_top_base_out", vec3(11.8499994,1.12,-17.9099998), vec3(0,-90,0), vec3(12.2506094,2,1));
    create_wall("Small_top_base_out_2", vec3(11.54,1.12,-17.8600006), vec3(0,90,0), vec3(12.2506094,2,1));

    create_wall("Bottom_base_out", vec3(-0.0999999046,1.12,40.15), vec3(0,180,0), vec3(17.2545071,2,1));
    create_wall("Bottom_base_out_2", vec3(-0.120000452,1.12,39.25), vec3(0,0,0), vec3(17.2545071,2,1));
    create_wall("Bottom_base_front", vec3(-13.5200005,1.12,27.27), vec3(0,0,0), vec3(32.0919456,2,1));
    create_wall("Bottom_base_front_2", vec3(-13.5,1.12,27.5699997), vec3(0,180,0), vec3(32.0919456,2,1));
    create_wall("Small_bottom_base", vec3(-8.83999729,1.11999893,36.409996), vec3(0,90,0), vec3(7.55187654,2,1));
    create_wall("Small_bottom_base_2", vec3(-8.49000072,1.11999893,36.4099998), vec3(0,-90,0), vec3(7.55187654,2,1));
    create_wall("Bottom_base_exit", vec3(2.04999971,1.11999893,31.7000008), vec3(0,90,0), vec3(8.63298225,2,1));
    create_wall("Bottom_base_exit_2", vec3(2.3999958,1.11999893,31.7000046), vec3(0,-90,0), vec3(8.63298225,2,1));

    create_wall("Top_ramp_wall_lower", vec3(-7.50000095,1.12,-29.75), vec3(0,90,0), vec3(23.8665085,2,1));
    create_wall("Top_ramp_wall_lower_2", vec3(-7.19000149,1.12,-29.7999992), vec3(0,-90,0), vec3(23.8665085,2,1));
    create_wall("Bottom_ramp_wall_lower", vec3(8.04999924,1.11999893,32), vec3(0,90,0), vec3(16.1505833,2,1));
    create_wall("Bottom_ramp_wall_lower_2", vec3(8.3999958,1.11999893,32.0000038), vec3(0,-90,0), vec3(16.1505833,2,1));
    create_wall("Bottom_ramp_wall_lower2", vec3(14,1.11999893,32.25), vec3(0,90,0), vec3(16.3098202,2,1));
    create_wall("Bottom_base_ramp_exit", vec3(17.5,1.12,40.2599983), vec3(0,180,0), vec3(6.84860229,2,1));
    create_wall("Top_ramp_wall_upper", vec3(-6.98,7,-30.25), vec3(0,-90,0), vec3(23.3202152,2,1));
    create_wall("Top_ramp_wall_upper_2", vec3(-7.50000095,7,-30.25), vec3(0,90,0), vec3(23.3271255,2,1));
    create_wall("Bottom_ramp_wall_upper", vec3(8.3999958,7,32.0000038), vec3(0,-90,0), vec3(16.1505833,2,1));
    create_wall("Bottom_ramp_wall_upper_2", vec3(8.04999924,7,32), vec3(0,90,0), vec3(16.1505833,2,1));

    create_wall("Courtyard_bottom", vec3(14.25,1.12,24.3400002), vec3(0,0,0), vec3(12.5768623,2,1));
    create_wall("Courtyard_top", vec3(7.05999947,1.12,-11.7199993), vec3(0,180,0), vec3(9.24054241,2,1));
    create_wall("Courtyard_top_2", vec3(7.05999947,1.12,-12.04), vec3(0,0,0), vec3(9.24054241,2,1));

    create_wall("back_wall1", vec3(21.8199997,1.12,49.9500008), vec3(0,0,0), vec3(15.5809641,2,1));
    create_wall("back_wall2", vec3(6.30000019,1.12,49.9500008), vec3(0,0,0), vec3(15.5809641,2,1));
    create_wall("back_wall3", vec3(-15.4700003,1.12,49.9500008), vec3(0,0,0), vec3(28,2,1));

    create_wall("top_wall1", vec3(0.129999995,1.12,-49.6399994), vec3(0,180,0), vec3(59.5112,2,1));

    create_wall("long_divider_wall_right", vec3(-20.8500004,1.11999893,-10.789999), vec3(0,90,0), vec3(62.4957161,2,1));
    create_wall("long_divider_wall_right_2", vec3(-20.5,1.11999893,-10.79), vec3(0,-90,0), vec3(62.4957161,2,1));

    create_wall("long_divider_wall_left", vec3(20.8799973,1.11999893,9.8000021), vec3(0,-90,0), vec3(60.6925163,2,1));
    create_wall("long_divider_wall_left_2", vec3(20.5299988,1.11999893,9.80000019), vec3(0,90,0), vec3(60.6925163,2,1));
    create_wall("off_wall_left", vec3(-11.75,1.12,-12.1899996), vec3(0,0,0), vec3(17.9672222,2,1));
    create_wall("off_wall_left_2", vec3(-11.75,1.12,-11.8699989), vec3(0,180,0), vec3(17.9672222,2,1));
    create_wall("courtyard_top_cover", vec3(-1.3500005,1.12,-5.30000019), vec3(0,0,0), vec3 (25.2183399,2,1));
    create_wall("courtyard_top_cover_2", vec3(-1.33000004,1.12,-4.93000078), vec3(0,180,0), vec3(25.2183399,2,1));
    create_wall("courtyard_bottom_cover", vec3(5.67999935,1.12,18), vec3(0,0,0), vec3(17.61059,2,1));
    create_wall("courtyard_bottom_cover_2", vec3(5.69999981,1.12,18.3699989), vec3(0,180,0), vec3(17.61059,2,1));

    create_wall("Left_wall_upper", vec3(20.04,7.0999999,3.00000048), vec3(0,90,0), vec3(43.8346291,2,1));
    create_wall("Left_wall_upper_2", vec3(21.04,7.0999999,2.75000048), vec3(0,-90,0), vec3(43.8346291,2,1));
    create_wall("Right_wall_upper", vec3(-20.21,7.0999999,2.86000061), vec3(0,-90,0), vec3(43.8346291,2,1));
    create_wall("Right_wall_upper_2", vec3(-20.75,7.0999999,2.75000048), vec3(0,90,0), vec3(43.8346291,2,1));
    create_wall("Bottom_wall_upper", vec3(-4.20999908,7.0999999,24.6200008), vec3(0,180,0), vec3(27.8976765,2,1));
    create_wall("Bottom_wall_upper_2", vec3(-6.45999908,7.0999999,24.3100014), vec3(0,0,0), vec3(28.6290627,2,1));
    create_wall("Top_wall_upper", vec3(6.54000092,7.0999999,-18.5600014), vec3(0,180,0), vec3(28.6290627,2,1));
    create_wall("Top_wall_upper_2", vec3(6.54000092,7.0999999,-19.0600014), vec3(0,0,0), vec3(28.6290627,2,1));
    create_wall("Top_wall_upper_small", vec3(-16.9599991,7.0999999,-19.0599995), vec3(0,0,0), vec3(7.23584938,2,1));
    create_wall("Top_wall_upper_small_2", vec3(-16.9399948,7.0999999,-18.75), vec3(0,180,0), vec3(7.23584938,2,1));
    create_wall("Bottom_wall_upper_small", vec3(17.2900009,7.0999999,24.1900005), vec3(0,0,0), vec3(7.23584938,2,1));
    create_wall("Bottom_wall_upper_small_2", vec3(17.3100052,7.0999999,24.5), vec3(0,180,0), vec3(7.23584938,2,1));

    create_wall("Central_wall_upper_left", vec3(7.56999969,7.0999999,2.68000054), vec3(0,90,0), vec3(29.467432,0.5,1));
    create_wall("Central_wall_upper_left_2", vec3(7.98999786,7.0999999,2.65000057), vec3(0,-90,0), vec3(29.467432,0.5,1));
    create_wall("Central_wall_upper_right", vec3(-8.45999908,7.0999999,2.68000054), vec3(0,90,0), vec3(29.7539444,0.5,1));
    create_wall("Central_wall_upper_right_2", vec3(-8.04000092,7.0999999,2.65000057), vec3(0,-90,0), vec3(29.7539444,0.5,1));
    create_wall("Central_wall_upper_top", vec3(0,7.0999999,-11.7499981), vec3(0,180,0), vec3(16,0.5,1));
    create_wall("Central_wall_upper_top_2", vec3(-0.0200042725,7.0999999,-12.0599995), vec3(0,0,0), vec3(16,0.5,1));
    create_wall("Central_wall_upper_bottom", vec3(-0.0200042725,7.0999999,16.9400005), vec3(0,0,0), vec3(16,0.5,1));
    create_wall("Central_wall_upper_bottom_2", vec3(0,7.0999999,17.25), vec3(0,180,0), vec3(16,0.5,1));

    create_player("player_1",vec3(-20,5,-20),vec3(0),0);
    create_player("player_2",vec3(20,5,-20),vec3(0),1);
    create_player("player_3",vec3(-20,5,20),vec3(0),2);
    create_player("player_4",vec3(20,5,20),vec3(0),3);
    create_player("player_5",vec3(0,5,-20),vec3(0),4);
    create_player("player_6",vec3(0,5,20),vec3(0),5);

    create_post_processing();
}
GameObject Untie::create_level_bottom() {
    auto cubeObj = m_scene->create_game_object("level_bottom");
    cubeObj.get_component<components::Transform>().set_position(vec3(0,-200,0));
    cubeObj.get_component<components::Transform>().set_scale(glm::vec3(0, 0, 0));
    cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 0, 0));
    cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

    auto& shape = cubeObj.add_component<components::Shape>();
    vec3 halfsize = vec3(vec3(0.1f, 0.1f, 0.1f));
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

}  // namespace knot
