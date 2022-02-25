#include <knoting/components.h>
#include <knoting/game_object.h>
#include <knoting/instance_mesh.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/spot_light.h>
#include <knoting/transform.h>
#include <cereal/archives/json.hpp>

namespace knot {

entt::registry& Scene::get_registry() {
    return m_registry;
}

Scene::~Scene() {
    while (!m_entityGameObjectMap.empty()) {
        auto it = m_entityGameObjectMap.begin();
        remove_game_object(it->second);
    }
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

void Scene::remove_game_object(GameObject game_object) {
    KNOTING_ASSERT_MESSAGE(m_registry.valid(game_object.m_handle),
                           "Trying to remove Game object with id {}, which is not valid",
                           to_string(game_object.get_id()));

    if (game_object.has_component<components::Hierarchy>()) {
        auto& hierarchy = game_object.get_component<components::Hierarchy>();
        if (hierarchy.has_children()) {
            for (auto& child : hierarchy.get_children()) {
                remove_game_object(child);
            }
        }

        auto parentOpt = hierarchy.get_parent();
        if (parentOpt) {
            GameObject parent = parentOpt.value();
            parent.get_component<components::Hierarchy>().remove_child(game_object);
        }
    }

    for (auto [id, pool] : m_registry.storage()) {
        using namespace entt::literals;

        if (!pool.contains(game_object.m_handle)) {
            continue;
        }

        auto meta = entt::resolve(id);
        if (!meta) {
            log::debug("Could not resolve meta for id {}", id);
            continue;
        }

        auto removeComponent = meta.func("on_destroy"_hs);
        if (!removeComponent) {
            log::debug("Could not find remove_component function for id {}", id);
            continue;
        }

        removeComponent.invoke(game_object.m_handle);
    }

    m_uuidGameObjectMap.erase(game_object.get_id());
    m_entityGameObjectMap.erase(game_object.m_handle);

    m_registry.destroy(game_object.m_handle);
    log::debug("Removed game object with id {}", to_string(game_object.get_id()));
}

std::optional<GameObject> Scene::get_game_object_from_id(uuid id) {
    auto it = m_uuidGameObjectMap.find(id);
    if (it == m_uuidGameObjectMap.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<GameObject> Scene::get_game_object_from_handle(entt::entity handle) {
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

template <typename T>
std::optional<GameObject> Scene::get_game_object_from_component(T& component) {
    auto sceneOpt = get_active_scene();
    if (!sceneOpt) {
        return std::nullopt;
    }

    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.m_registry, component);
    return scene.get_game_object_from_handle(handle);
}
void Scene::save_scene_to_stream() {
    cereal::JSONOutputArchive archive(m_serializedScene);
    entt::snapshot{m_registry}
        .entities(archive)
        .component<uuid, components::Name, components::Tag, components::Transform, components::Material,
                   components::InstanceMesh, components::SpotLight, components::EditorCamera>(archive);
}
void Scene::load_scene_from_stream(std::stringstream serialized) {
    m_registry.clear();
    m_uuidGameObjectMap.clear();
    m_entityGameObjectMap.clear();

    cereal::JSONInputArchive archive(serialized);
    entt::snapshot_loader{m_registry}
        .entities(archive)
        .component<uuid, components::Name, components::Tag, components::Transform, components::Material,
                   components::InstanceMesh, components::SpotLight, components::EditorCamera>(archive);
    auto view = m_registry.view<uuid>();
    for (auto ent : view) {
        add_game_object(ent);
    }
    m_serializedScene.clear();
}

GameObject Scene::add_game_object(entt::entity handle) {
    GameObject e(handle, *this);

    m_uuidGameObjectMap.insert(std::make_pair(e.get_id(), e));
    m_entityGameObjectMap.insert(std::make_pair(e.m_handle, e));

    return e;
}

}  // namespace knot
