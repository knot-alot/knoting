#include <knoting/engine.h>
#include <knoting/px_material.h>

namespace knot {
namespace components {
PhysicsMaterial::PhysicsMaterial() : m_physics(nullptr), m_material(nullptr) {}

PhysicsMaterial::~PhysicsMaterial() {}

void PhysicsMaterial::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_physics = engine.get_physics_module().lock()->get_physics().lock();
    }
    constexpr PxReal defalut_staticFriction = 0.3f;
    constexpr PxReal defalut_dynamicFriction = 0.3f;
    constexpr PxReal defalut_restitution = 0.6f;
    m_material = std::make_shared<PxMaterial_ptr_wrapper>(
        m_physics->get()->createMaterial(defalut_staticFriction, defalut_dynamicFriction, defalut_restitution));
}

void PhysicsMaterial::on_destroy() {}

void PhysicsMaterial::set_px_material(float staticFriction, float dynamicFriction, float restitution) {
    m_material->get()->setDynamicFriction(dynamicFriction);
    m_material->get()->setStaticFriction(staticFriction);
    m_material->get()->setRestitution(restitution);
}

}  // namespace components
}  // namespace knot
