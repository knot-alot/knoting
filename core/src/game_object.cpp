#include <knoting/game_object.h>
#include <knoting/log.h>
#include <iostream>

namespace knot {

namespace components {

Hierarchy::Hierarchy() : m_parent(std::nullopt), m_children({}) {}
Hierarchy::Hierarchy(GameObject parent) : m_parent(parent.get_id()), m_children({}) {}
Hierarchy::Hierarchy(GameObject parent, const std::vector<GameObject>& children) : m_parent(parent.get_id()) {
    for (GameObject gObj : children)
        m_children.push_back(gObj.get_id());
}
Hierarchy::Hierarchy(const std::vector<GameObject>& children) : m_parent(std::nullopt) {
    for (GameObject gObj : children)
        m_children.push_back(gObj.get_id());
}

std::optional<uuid> Hierarchy::get_parent() const {
    return m_parent;
}

bool Hierarchy::has_children() const {
    return !m_children.empty();
}

std::vector<uuid> Hierarchy::get_children() const {
    return m_children;
}

void Hierarchy::add_child(GameObject child) {
    m_children.push_back(child.get_id());
}

void Hierarchy::remove_child(GameObject child) {
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child.get_id()), m_children.end());
}

Tag::Tag(const std::string& tag) : m_id(0) {
    if (tag.empty()) {
        return;
    }

    auto it = s_tags.find(tag);
    if (it == s_tags.end()) {
        log::debug("Tag '{}' not found", tag);
        return;
    }

    m_id = it->second;
}

uint16_t Tag::get_id() const {
    return m_id;
}

std::string Tag::get_tag() const {
    auto it = s_ids.find(m_id);
    if (it == s_ids.end()) {
        return "";
    }
    return it->second;
}

void Tag::register_tag(const std::string& tag) {
    if (tag.empty()) {
        return;
    }

    auto it = s_tags.find(tag);
    if (it != s_tags.end()) {
        return;
    }

    uint16_t id = s_next_id++;
    s_tags.insert({tag, id});
    s_ids.insert({id, tag});
}

void Tag::unregister_tag(const std::string& tag) {
    if (tag.empty()) {
        return;
    }

    auto it = s_tags.find(tag);
    if (it == s_tags.end()) {
        return;
    }

    s_tags.erase(it->first);
    s_ids.erase(it->second);
}

std::vector<std::string> Tag::get_registered_tags() {
    std::vector<std::string> tags;
    for (const auto& tag : s_tags) {
        tags.push_back(tag.first);
    }
    return tags;
}

}  // namespace components

UUIDGenerator::UUIDGenerator() {
    std::random_device randomDevice;
    std::array<int, std::mt19937::state_size> seedData;
    std::generate(std::begin(seedData), std::end(seedData), std::ref(randomDevice));
    std::seed_seq seedSequences(std::begin(seedData), std::end(seedData));

    m_randomGenerator = std::mt19937(seedSequences);
    m_uuidGenerator = std::make_unique<uuids::uuid_random_generator>(m_randomGenerator);
}

uuid UUIDGenerator::generate() {
    return (*m_uuidGenerator)();
}

GameObject::GameObject(entt::entity handle, Scene& scene) : m_handle(handle), m_scene(scene) {
    this->try_add_component<uuid>(s_uuidGenerator.generate());
}

const uuid GameObject::get_id() const {
    return this->get_component<uuid>();
}

bool GameObject::has_no_components() const {
    return m_scene.get().m_registry.orphan(m_handle);
}
const entt::entity GameObject::get_handle() const {
    return m_handle;
}

}  // namespace knot
