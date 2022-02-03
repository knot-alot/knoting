#pragma once

#include <knoting/assert.h>
#include <knoting/scene.h>
#include <knoting/transform.h>
#include <knoting/types.h>
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

template <typename T>
class HasOnAwake {
    typedef char one;
    struct two {
        char x[2];
    };

    template <typename C>
    static one test(decltype(&C::on_awake));
    template <typename C>
    static two test(...);

   public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

template <typename T>
class HasOnDestroy {
    typedef char one;
    struct two {
        char x[2];
    };

    template <typename C>
    static one test(decltype(&C::on_destroy));
    template <typename C>
    static two test(...);

   public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

class GameObject {
   public:
    GameObject(entt::entity handle, Scene& scene);

    inline bool operator==(const GameObject& other) const { return m_id == other.m_id; }
    inline bool operator!=(const GameObject& other) const { return m_id != other.m_id; }

    const uuid get_id() const;
    const entt::entity get_handle()const;
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
    void remove_component() {
        KNOTING_ASSERT_MESSAGE(has_component<T>(), "GameObject does not have component");

        T& t = get_component<T>();
        call_on_destroy(t);
        m_scene.get().m_registry.remove<T>(m_handle);
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
    typename enable_if<HasOnAwake<T>::value>::type call_on_awake(T& t) {
        t.on_awake();
    }

    template <typename T>
    typename enable_if<!HasOnAwake<T>::value>::type call_on_awake(T&) {}

    template <typename T>
    typename enable_if<HasOnDestroy<T>::value>::type call_on_destroy(T& t) {
        t.on_destroy();
    }

    template <typename T>
    typename enable_if<!HasOnDestroy<T>::value>::type call_on_destroy(T&) {}

    uuid m_id;
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

    std::optional<GameObject> get_parent() const;

    bool has_children() const;
    std::vector<GameObject> get_children() const;
    void add_child(GameObject child);
    void remove_child(GameObject child);

   protected:
    std::optional<GameObject> m_parent;
    std::vector<GameObject> m_children;
};

class Name {
   public:
    std::string name;

    Name(const std::string& name) : name(name) {}
    Name() : name(DEFAULT_NAME) {}

    inline bool operator==(const Name& other) const { return name == other.name; }
    inline bool operator!=(const Name& other) const { return name != other.name; }

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

   protected:
    uint16_t m_id;

    inline static std::map<std::string, uint16_t> s_tags;
    inline static std::map<uint16_t, std::string> s_ids;
    inline static uint16_t s_next_id = 1;
};

}  // namespace components

}  // namespace knot
