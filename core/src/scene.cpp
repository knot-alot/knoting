#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/scene.h>
#include <knoting/transform.h>

namespace knot {

GameObject Scene::create_game_object(const std::string& name) {
    GameObject e(m_registry.create(), *this);
    e.add_component<components::Transform>();
    e.add_component<components::Hierarchy>();

    std::string n = name.empty() ? components::Name::DEFAULT_NAME : name;
    e.add_component<components::Name>(n);

    m_uuidGameObjectMap.insert(std::make_pair(e.m_id, e));
    m_entityGameObjectMap.insert(std::make_pair(e.m_handle, e));

    log::debug("Created game object with id {}", to_string(e.m_id));

    return e;
}

void Scene::remove_game_object(GameObject game_object) {
    KNOTING_ASSERT_MESSAGE(m_registry.valid(game_object.m_handle),
                           "Trying to remove Game object with id {}, which is not valid", to_string(game_object.m_id));

    if (game_object.has_component<components::Hierarchy>()) {
        auto& hierarchy = game_object.get_component<components::Hierarchy>();
        if (hierarchy.has_children()) {
            for (auto& child : hierarchy.get_children()) {
                remove_game_object(child);
            }
        }

        auto parent_opt = hierarchy.get_parent();
        if (parent_opt) {
            GameObject parent = parent_opt.value();
            parent.get_component<components::Hierarchy>().remove_child(game_object);
        }
    }

    m_uuidGameObjectMap.erase(game_object.m_id);
    m_entityGameObjectMap.erase(game_object.m_handle);

    m_registry.destroy(game_object.m_handle);
    log::debug("Removed game object with id {}", to_string(game_object.m_id));
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

void Scene::set_active_scene(Scene& scene) {
    s_activeScene = std::ref(scene);
}

template <typename T>
std::optional<GameObject> Scene::get_game_object_from_component(T& component) {
    auto scene_opt = get_active_scene();
    if (!scene_opt) {
        return std::nullopt;
    }

    Scene& scene = scene_opt.value();
    entt::entity handle = entt::to_entity(scene.m_registry, component);
    return scene.get_game_object_from_handle(handle);
}

entt::registry& Scene::get_registry() const {
    return m_registry;
}

}  // namespace knot
