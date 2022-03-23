#include <knoting/engine.h>
#include <knoting/raycast.h>
#include <knoting/rigidbody.h>
#include <cereal/archives/json.hpp>

namespace knot {
namespace components {
Raycast::Raycast() : m_scene(nullptr) {}

Raycast::~Raycast() {}

void Raycast::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_scene = engine.get_physics_module().lock()->get_active_Scene().lock();
    }
    m_isHit = false;
    m_origin = get_position_from_transform();
}

void Raycast::raycast() {
    m_scene->get()->raycast(m_origin, m_unitDir, m_maxDistance, m_hit);
}

vec3 Raycast::get_origin() {
    return RigidBody::PxVec3_to_vec3(m_origin);
}

vec3 Raycast::get_unitDir() {
    return RigidBody::PxVec3_to_vec3(m_unitDir);
}

vec3 Raycast::get_hit_position() {
    if (m_isHit) {
        return RigidBody::PxVec3_to_vec3(m_hit.block.position);
    }
    return vec3(std::numeric_limits<float>::max());
}

vec3 Raycast::get_hit_normal() {
    if (m_isHit) {
        return RigidBody::PxVec3_to_vec3(m_hit.block.normal);
    }
    return vec3(std::numeric_limits<float>::max());
}

float Raycast::get_hit_distance() {
    if (m_isHit) {
        return m_hit.block.distance;
    }
    return std::numeric_limits<float>::max();
}

std::weak_ptr<PxShape_ptr_wrapper> Raycast::get_hit_shape() {
    if (m_isHit) {
        return std::make_shared<PxShape_ptr_wrapper>(m_hit.block.shape);
    }
    return std::make_shared<PxShape_ptr_wrapper>();
}

void Raycast::set_origin(const vec3& origin) {
    m_origin = RigidBody::vec3_to_PxVec3(origin);
}
void Raycast::set_unit_dir(const vec3& unitDir) {
    m_unitDir = RigidBody::vec3_to_PxVec3(unitDir);
}

void Raycast::set_max_distance(const float& maxDistance) {
    m_maxDistance = maxDistance;
}

void Raycast::set_raycast(const vec3& origin, const vec3& unitDir, const float& maxDistance) {
    m_origin = RigidBody::vec3_to_PxVec3(origin);
    m_unitDir = RigidBody::vec3_to_PxVec3(unitDir);
    m_maxDistance = maxDistance;
}

PxVec3 Raycast::get_position_from_transform() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);

        if (goOpt) {
            PxVec3 position = RigidBody::vec3_to_PxVec3(goOpt->get_component<components::Transform>().get_position());

            return position;
        }
    }
    return PxVec3(std::numeric_limits<float>::max());
}

template <class Archive>
void Raycast::save(Archive& archive) const {
    archive(CEREAL_NVP(RigidBody::PxVec3_to_vec3(m_origin)), CEREAL_NVP(RigidBody::PxVec3_to_vec3(m_unitDir)),
            CEREAL_NVP((float)m_maxDistance));
}

template <class Archive>
void Raycast::load(Archive& archive) {
    vec3 origin, unitDir;
    float maxDistance;
    archive(CEREAL_NVP(origin), CEREAL_NVP(unitDir), CEREAL_NVP(maxDistance));
    m_origin = RigidBody::vec3_to_PxVec3(origin);
    m_unitDir = RigidBody::vec3_to_PxVec3(unitDir);
    m_maxDistance = maxDistance;
    on_awake();
}

template void Raycast::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
template void Raycast::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
}  // namespace components
}  // namespace knot