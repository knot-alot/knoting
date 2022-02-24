#include <knoting/engine.h>
#include <knoting/raycast.h>
#include <knoting/rigidbody.h>

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
    m_ishit = false;
    // get pos
}

void Raycast::on_destroy() {}

void Raycast::raycast() {
    m_scene->get()->raycast(m_origin, m_unitDir, m_maxDistance, m_hit);
}

vec3 Raycast::get_origin() {
    return RigidBody::PxVec3_to_vec3(m_origin);
}

vec3 Raycast::get_unitDir() {
    return RigidBody::PxVec3_to_vec3(m_unitDir);
}

vec3 Raycast::get_hitPosition() {
    if (m_ishit) {
        return RigidBody::PxVec3_to_vec3(m_hit.block.position);
    }
    return vec3(std::numeric_limits<float>::max());
}

vec3 Raycast::get_hitNomal() {
    if (m_ishit) {
        return RigidBody::PxVec3_to_vec3(m_hit.block.normal);
    }
    return vec3(std::numeric_limits<float>::max());
}

float Raycast::get_hitDistance() {
    if (m_ishit) {
        return m_hit.block.distance;
    }
    return std::numeric_limits<float>::max();
}

std::weak_ptr<PxShape_ptr_wrapper> Raycast::get_hitShape() {
    if (m_ishit) {
        return std::make_shared<PxShape_ptr_wrapper>(m_hit.block.shape);
    }
    return std::make_shared<PxShape_ptr_wrapper>();
}

}  // namespace components
}  // namespace knot
