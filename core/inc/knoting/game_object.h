#pragma once

#include <knoting/assert.h>
#include <knoting/component.h>
#include <knoting/scene.h>
#include <knoting/transform.h>
#include <knoting/types.h>
#include <cereal/cereal.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entt.hpp>
#include <random>

namespace knot {

class Scene;

}  // namespace knot

namespace knot {

class UUIDGenerator {
   public:
    UUIDGenerator();

    uuid generate();

   protected:
    std::unique_ptr<uuids::uuid_random_generator> m_uuidGenerator;
    std::mt19937 m_randomGenerator;
};

class GameObject {
   public:
    GameObject(entt::entity handle, Scene& scene);

    inline bool operator==(const GameObject& other) const { return this->get_id() == other.get_id(); }
    inline bool operator!=(const GameObject& other) const { return this->get_id() != other.get_id(); }

    const uuid get_id() const;
    const entt::entity get_handle() const;
    bool has_no_components() const;

    template <typename... Components>
    bool has_component() const {
        return m_scene.get().m_registry.all_of<Components...>(m_handle);
    }

    template <typename... Components>
    bool has_any_of() const {
        return m_scene.get().m_registry.any_of<Components...>(m_handle);
    }

    template <typename T, typename... Args>
    T& add_component(Args&&... args) {
        KNOTING_ASSERT_MESSAGE(!has_component<T>(), "GameObject already has component");

        T& t = m_scene.get().m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
        call_on_awake(t);
        return t;
    }

    template <typename T, typename... Args>
    T& try_add_component(Args&&... args) {
        if (has_component<T>())
            return get_component<T>();

        return add_component<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    T& get_component() {
        KNOTING_ASSERT_MESSAGE(has_component<T>(), "GameObject does not have component");

        return m_scene.get().m_registry.get<T>(m_handle);
    }

    template <typename T>
    const T& get_component() const {
        KNOTING_ASSERT_MESSAGE(has_component<T>(), "GameObject does not have component");

        return m_scene.get().m_registry.get<T>(m_handle);
    }

    template <typename T>
    static std::optional<GameObject> get_game_object_from_component(T& component) {
        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt) {
            return std::nullopt;
        }

        Scene& scene = sceneOpt.value();
        entt::registry& registry = scene.m_registry;
        entt::entity handle = entt::to_entity(registry, component);
        return scene.get_game_object_from_handle(handle);
    }

    template <typename T>
    void remove_component() {
        KNOTING_ASSERT_MESSAGE(has_component<T>(), "GameObject does not have component");

        T& t = get_component<T>();
        call_on_destroy(t);
        m_scene.get().m_registry.remove<T>(m_handle);
    }

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(this->get_id()));
    }

   protected:
    friend class Scene;

    template <bool C, typename T = void>
    struct enable_if {
        typedef T type;
    };

    template <typename T>
    struct enable_if<false, T> {};

    template <typename T>
    typename enable_if<std::is_base_of<Component<T>, T>::value>::type call_on_awake(T& t) {
        t.on_awake();
    }

    template <typename T>
    typename enable_if<!std::is_base_of<Component<T>, T>::value>::type call_on_awake(T& t) {}

    template <typename T>
    typename enable_if<std::is_base_of<Component<T>, T>::value>::type call_on_destroy(T& t) {
        t.on_destroy();
    }

    template <typename T>
    typename enable_if<!std::is_base_of<Component<T>, T>::value>::type call_on_destroy(T&) {}

    entt::entity m_handle;

    std::reference_wrapper<Scene> m_scene;

    inline static UUIDGenerator s_uuidGenerator;
};

namespace components {

class Hierarchy {
   public:
    Hierarchy();
    Hierarchy(GameObject parent);
    Hierarchy(GameObject parent, const std::vector<GameObject>& children);
    Hierarchy(const std::vector<GameObject>& children);

    std::optional<uuid> get_parent() const;
    bool has_parent() const;
    void set_parent(GameObject parent);

    bool has_children() const;
    bool has_child(uuid id) const;
    std::vector<uuid> get_children() const;
    void add_child(GameObject child);
    void remove_child(GameObject child);

    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(m_parent), CEREAL_NVP(m_children));
    }

   protected:
    uuid m_parent;
    std::vector<uuid> m_children;
};

class Name {
   public:
    std::string name;

    Name(const std::string& name) : name(name) {}
    Name() : name(DEFAULT_NAME) {}

    inline bool operator==(const Name& other) const { return name == other.name; }
    inline bool operator!=(const Name& other) const { return name != other.name; }
    std::string get_name() { return name; }
    template <class Archive>
    void serialize(Archive& archive) {
        archive(CEREAL_NVP(name));
    }

   private:
    friend class knot::Scene;

    static constexpr const char* DEFAULT_NAME = "GameObject";
};

class Tag {
   public:
    Tag(const std::string& tag = "");

    uint16_t get_id() const;
    std::string get_tag() const;

    inline bool operator==(const Tag& other) const { return m_id == other.m_id; }
    inline bool operator!=(const Tag& other) const { return m_id != other.m_id; }

    static void register_tag(const std::string& tag);
    static void unregister_tag(const std::string& tag);
    static std::vector<std::string> get_registered_tags();

    template <class Archive>
    void serialize(Archive& archive) {
        archive(this->get_registered_tags());
    }

   protected:
    uint16_t m_id;

    inline static std::map<std::string, uint16_t> s_tags;
    inline static std::map<uint16_t, std::string> s_ids;
    inline static uint16_t s_next_id = 1;
};

}  // namespace components

}  // namespace knot
