#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/post_processing.h>
#include <knoting/scene.h>
#include <knoting/skybox.h>
#include <knoting/spot_light.h>
#include <knoting/transform.h>
#include <cereal/archives/json.hpp>

namespace knot {

entt::registry& Scene::get_registry() {
    return m_registry;
}

Scene::~Scene() {
    unload();
}

void Scene::unload() {
    while (!m_entityGameObjectMap.empty()) {
        auto it = m_entityGameObjectMap.begin();
        remove_game_object(it->second);
    }

    m_registry.clear();
    m_uuidGameObjectMap.clear();
    m_entityGameObjectMap.clear();
}

GameObject Scene::create_game_object(const std::string& name) {
    GameObject e(m_registry.create(), *this);
    e.add_component<components::Transform>();
    e.add_component<components::Hierarchy>();

    std::string n = name.empty() ? components::Name::DEFAULT_NAME : name;
    e.add_component<components::Name>(n);

    m_uuidGameObjectMap.insert(std::make_pair(e.get_id(), e));
    m_entityGameObjectMap.insert(std::make_pair(e.m_handle, e));

    log::debug("Created game object with id {}", to_string(e.get_id()));

    return e;
}

GameObject Scene::create_bullet(bool is_teamA, vec3 spawnPos) {
    GameObject e(m_registry.create(), *this);
    e.add_component<components::Transform>();
    e.add_component<components::Hierarchy>();
    std::string n;
    is_teamA ? n = "RED" : n = "BLUE";
    e.add_component<components::Name>(n);
    e.add_component<components::InstanceMesh>("uv_cube.obj");

    m_uuidGameObjectMap.insert(std::make_pair(e.get_id(), e));
    m_entityGameObjectMap.insert(std::make_pair(e.m_handle, e));

    log::debug("Created game object with id {}", to_string(e.get_id()));

    e.add_component<components::PhysicsMaterial>();

    auto& shape = e.add_component<components::Shape>();
    shape.set_geometry(shape.create_sphere_geometry(0.2f));
    is_teamA ? shape.set_filter_data(filter_group::eParticle_red, filter_group::eAll | filter_group::ePlayer_blue)
             : shape.set_filter_data(filter_group::eParticle_blue, filter_group::eAll | filter_group::ePlayer_red);

    auto& rigidBody = e.add_component<components::RigidBody>();
    rigidBody.create_actor(true, 1.0f);
    rigidBody.set_name(n);

    e.add_component<components::RigidController>();
    auto& detection = e.add_component<components::Collision_Detection>();
    detection.add_search_actor(rigidBody.get_dynamic().lock());

    auto material = components::Material();
    material.set_texture_slot_path(TextureType::Albedo, "UV_Grid_test.png");
    material.set_texture_slot_path(TextureType::Normal, "normal_tiles_1k.png");
    material.set_texture_slot_path(TextureType::Metallic, "whiteTexture");
    material.set_texture_slot_path(TextureType::Roughness, "whiteTexture");
    material.set_texture_slot_path(TextureType::Occlusion, "whiteTexture");
    e.add_component<components::Material>(material);

    e.add_component<components::InstanceScript>("bullet.js");
    return e;
}

void Scene::remove_game_object(GameObject game_object) {
    KNOTING_ASSERT_MESSAGE(m_registry.valid(game_object.m_handle),
                           "Trying to remove Game object with id {}, which is not valid",
                           to_string(game_object.get_id()));

    uuid id = game_object.get_id();

    if (game_object.has_component<components::Hierarchy>()) {
        auto& hierarchy = game_object.get_component<components::Hierarchy>();

        auto parentIdOpt = hierarchy.get_parent();
        if (parentIdOpt) {
            auto parentOpt = get_game_object_from_id(parentIdOpt.value());
            if (parentOpt) {
                parentOpt.value().get_component<components::Hierarchy>().remove_child(game_object);
            }
        }

        if (hierarchy.has_children()) {
            for (auto& childId : hierarchy.get_children()) {
                auto childOpt = get_game_object_from_id(childId);
                if (!childOpt) {
                    continue;
                }
                remove_game_object(childOpt.value());
            }
        }
    }

    m_uuidGameObjectMap.erase(game_object.get_id());
    m_entityGameObjectMap.erase(game_object.m_handle);

    m_registry.destroy(game_object.m_handle);

    log::debug("Removed game object with id {}", to_string(id));
}

std::optional<GameObject> Scene::get_game_object_from_id(uuid id) {
    if (id.is_nil())
        return std::nullopt;

    auto it = m_uuidGameObjectMap.find(id);
    if (it == m_uuidGameObjectMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<GameObject> Scene::get_game_object_from_handle(entt::entity handle) {
    if (handle == entt::null || handle == entt::tombstone)
        return std::nullopt;

    auto it = m_entityGameObjectMap.find(handle);
    if (it == m_entityGameObjectMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<std::reference_wrapper<Scene>> Scene::get_active_scene() {
    return s_activeScene;
}

void Scene::set_active_scene(std::optional<std::reference_wrapper<Scene>> scene) {
    s_activeScene = std::ref(scene);
}

void Scene::save_scene_to_stream(std::ostream& serialized) {
    cereal::JSONOutputArchive archive(serialized);
    // TODO: When you make a new component add it here to the snapshot if it needs to be
    entt::snapshot{m_registry}
        .entities(archive)
        .component<uuid, components::Name, components::Tag, components::Transform, components::Hierarchy,
                   components::Material, components::SkyBox, components::InstanceMesh, components::SpotLight,
                   components::EditorCamera, components::PhysicsMaterial, components::Shape, components::RigidBody,
                   components::RigidController, components::Raycast, components::AudioSource, components::AudioListener,
                   components::PostProcessing>(archive);

    log::debug("Scene: Save Finished");
}
void Scene::load_scene_from_stream(std::istream& serialized) {
    m_uuidGameObjectMap.clear();
    m_entityGameObjectMap.clear();
    m_registry.clear();

    cereal::JSONInputArchive archive(serialized);
    // TODO: When you make a new component add it here to the snapshot if it needs to be
    entt::continuous_loader sceneLoader(m_registry);
    sceneLoader.entities(archive).component<uuid>(archive);
    auto view = m_registry.view<uuid>();
    for (auto ent : view) {
        add_game_object(ent);
    }
    sceneLoader.component<components::Name, components::Tag, components::Transform, components::Hierarchy,
                          components::Material, components::SkyBox, components::InstanceMesh, components::SpotLight,
                          components::EditorCamera, components::PhysicsMaterial, components::Shape,
                          components::RigidBody, components::RigidController, components::Raycast,
                          components::AudioSource, components::AudioListener, components::PostProcessing>(archive);

    // I know this is horrible but it's already full jank time. Can go back and be rewritten using the meta system
    auto ents = m_registry.view<components::Shape, components::RigidBody>();
    for (auto ent : ents) {
        auto goOpt = this->get_game_object_from_handle(ent);
        if (!goOpt) {
            continue;
        }
        auto go = goOpt.value();
        go.get_component<components::Shape>().on_load();
        go.get_component<components::RigidBody>().on_load();
    }

    auto entsRigCont = m_registry.view<components::RigidController>();
    for (auto ent : entsRigCont) {
        auto goOpt = this->get_game_object_from_handle(ent);
        if (!goOpt) {
            continue;
        }
        auto go = goOpt.value();
        go.get_component<components::RigidController>().on_load();
    }

    log::debug("Scene: Load Finished");
}

GameObject Scene::add_game_object(entt::entity handle) {
    GameObject e(handle, *this);

    m_uuidGameObjectMap.insert(std::make_pair(e.get_id(), e));
    m_entityGameObjectMap.insert(std::make_pair(e.m_handle, e));

    return e;
}

void Scene::add_to_postLoadBuffer(std::function<void()> func) {
    postLoadBuffer.emplace_back(func);
}

}  // namespace knot
