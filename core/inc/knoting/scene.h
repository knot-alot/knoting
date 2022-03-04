#pragma once

#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <entt/entt.hpp>
#include <optional>

namespace knot {

class GameObject;

}

namespace knot {

class Scene {
   public:
    ~Scene();

    GameObject create_game_object(const std::string& name = "");
    GameObject add_game_object(entt::entity handle);
    void remove_game_object(GameObject game_object);
    std::optional<GameObject> get_game_object_from_id(uuid id);
    std::optional<GameObject> get_game_object_from_handle(entt::entity handle);
    entt::registry& get_registry();

    template <typename T>
    static std::optional<GameObject> get_game_object_from_component(T& component);

    static std::optional<std::reference_wrapper<Scene>> get_active_scene();
    static void set_active_scene(std::optional<std::reference_wrapper<Scene>> scene);

    void save_scene_to_stream(std::ostream& serialized);
    void load_scene_from_stream(std::istream& serializedSc);

    static void add_to_postLoadBuffer(std::function<void()> func);

   protected:
    friend class GameObject;

    entt::registry m_registry;
    std::map<uuid, GameObject> m_uuidGameObjectMap;
    std::map<entt::entity, GameObject> m_entityGameObjectMap;

    inline static std::vector<std::function<void()>> postLoadBuffer;

    inline static std::optional<std::reference_wrapper<Scene>> s_activeScene = std::nullopt;
};

}  // namespace knot
