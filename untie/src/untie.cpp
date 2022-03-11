#include "untie.h"
#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/scene.h>
#include <knoting/texture.h>
#include <knoting/font_manager.h>
#include <knoting/text_buffer_manager.h>
#include <knoting/Font.h>
#include <knoting/entry.h>

#include <knoting/components.h>
#include <knoting/scene.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <fstream>
#include <iostream>

#include <cereal/archives/json.hpp>

#include <iostream>
namespace {
TrueTypeHandle loadTtf(FontManager* _fm, const char* _filePath) {
    uint32_t size;
    void* data = font::load(_filePath, &size);

    if (NULL != data) {
        TrueTypeHandle handle = _fm->createTtf((uint8_t*)data, size);
        BX_FREE(entry::getAllocator(), data);
        return handle;
    }

    TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
    return invalid;
}
}
namespace knot {
Scene scene;
Scene loadedScene;

Untie::Untie() {
    uint32_t size;
    Scene::set_active_scene(scene);
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();
    FontManager* m_fontManager = new FontManager(512);
    TextBufferManager* m_textBufferManager = new TextBufferManager(m_fontManager);
    

    //FontHandle m_fonts = m_fontManager->createFontByPixelSize(m_fontFiles,0,32);
  //  m_fontManager->preloadGlyph(m_fonts, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ. \n");
  //  TextBufferHandle m_staticText = m_textBufferManager->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Static);
  //  m_textBufferManager->setPenPosition(m_staticText, 24.0f, 100.0f);
  //  m_textBufferManager->appendText(m_staticText, m_fonts, L"The quick brown fox jumps over the lazy dog\n");
    Engine::set_active_engine(*m_engine);
    {
        auto editorCamera = scene.create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(-10.0f, 15.0f, -30.0f));
    }
    {
        auto cubeObj = scene.create_game_object("skybox");
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(30, 30, 30));
        cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

        auto skybox = components::SkyBox();
        skybox.set_texture_slot_path(SkyBoxTextureType::SkyBox, "skybox/cmft_skybox.hdr");
        skybox.set_texture_slot_path(SkyBoxTextureType::Irradiance, "skybox/cmtr_irradiance.hdr");
        skybox.set_texture_slot_path(SkyBoxTextureType::Radiance, "skybox/cmtr_radiance.hdr");
        cubeObj.add_component<components::SkyBox>(skybox);
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
        auto cubeObj = scene.create_game_object("floor");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f, 0.0f, -10.0f));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15, 1, 15));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 45, 0));
        cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(15.0, 1.0f, 15.0);
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
    }

    {
        auto cubeObj = scene.create_game_object("cube_1");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0.0f, 3.0f, 0.0f));
        cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f, 1.0f, 1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(true, 5.0f);

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }

    {
        auto cubeObj = scene.create_game_object("cube_2");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(1.0f, 7.0f, 1.0f));
        cubeObj.add_component<components::InstanceMesh>("uv_cube.obj");

        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();
        rigidbody.create_actor(true, 5.0f);

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
        material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);
    }
    {
        auto cubeObj = scene.create_game_object("stanford_dragon_1");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(-5.0f + 5, 5.0f, -10.0f - 5));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(3, 3, 3));
        cubeObj.get_component<components::Transform>().set_rotation_euler(glm::vec3(0, 240, 0));
        cubeObj.add_component<components::InstanceMesh>("dragon.obj");

        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.5f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(true, 4.0f);

        auto material = components::Material();
        material.set_texture_slot_path(TextureType::Albedo, "oldiron/OldIron01_1K_BaseColor.png");
        material.set_texture_slot_path(TextureType::Normal, "oldiron/OldIron01_1K_Normal.png");
        material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
        material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
        material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
        cubeObj.add_component<components::Material>(material);

    }
    std::string filename("skyboxScene.json");
    std::filesystem::path path = AssetManager::get_resources_path().append(filename);
    std::fstream serializedSceneStream(path);

    serializedSceneStream.open(path, std::ios_base::out);
    scene.save_scene_to_stream(serializedSceneStream);
    serializedSceneStream.close();
    Scene::set_active_scene(loadedScene);
    serializedSceneStream.open(path, std::ios_base::in);
    if (serializedSceneStream) {
        loadedScene.load_scene_from_stream(serializedSceneStream);
    } else {
        log::debug("file not found");
    }
    void* data = font::load("skybox/cmft_skybox.hdr", &size);

    
   
    bgfx::dbgTextClear();
    // abcdABCD
    //
    // 15 white
    // 14 Yellow
    // 12 lightred
    if (NULL != data) {
        bgfx::dbgTextPrintf(10, 10, 0x0f,
                            "\x1b[15;ma\x1b[10;mb\x1b[11;mc\x1b[12;md"    // abcd
                            "\x1b[7;mA\x1b[14; mB\x1b[8; mC\x1b[13;mF");  // ABCD
    }
  
    bgfx::setDebug(BGFX_DEBUG_TEXT); 
    serializedSceneStream.close();
}
void Untie::run() {

    while (m_engine->is_open()) {
        m_engine->update_modules();
        auto im = m_engine->get_window_module().lock()->get_input_manager();
        if (im.key_pressed(KeyCode::Escape)) {
            m_engine->get_window_module().lock()->close();
        }
    }
}

}  // namespace knot
