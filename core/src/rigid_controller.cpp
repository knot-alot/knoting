#include <knoting/engine.h>
#include <knoting/rigid_controller.h>
#include <knoting/rigidbody.h>

namespace knot {
namespace components {
RigidController::RigidController() : m_dynamic(nullptr) {}
RigidController::~RigidController() {}

void RigidController::on_awake() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            if (goOpt->has_component<components::RigidBody>()) {
                RigidBody& rb = goOpt->get_component<components::RigidBody>();
                m_dynamic = rb.get_dynamic().lock();
            }
        }
    }
    m_isKinematic = false;
    m_isAwake = true;
}

void RigidController::on_destroy() {}

std::weak_ptr<PxDynamic_ptr_wrapper> RigidController::get_dynamic() {
    return m_dynamic;
}

float RigidController::get_mass() {
    return m_dynamic->get()->getMass();
}

bool RigidController::get_isKinematic() {
    return m_isKinematic;
}

bool RigidController::get_isAwake() {
    return m_isAwake;
}

float RigidController::get_linear_damping() {
    return m_dynamic->get()->getLinearDamping();
}

float RigidController::get_angular_damping() {
    return m_dynamic->get()->getAngularDamping();
}

vec3 RigidController::get_linear_velocity() {
    return RigidBody::PxVec3_to_vec3(m_dynamic->get()->getLinearVelocity());
}

vec3 RigidController::get_angular_velocity() {
    return RigidBody::PxVec3_to_vec3(m_dynamic->get()->getAngularVelocity());
}

void RigidController::set_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> dynamic) {
    m_dynamic = dynamic;
}

void RigidController::set_kinematic(const bool& isKinematic) {
    m_isKinematic = isKinematic;
    m_dynamic->get()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, m_isKinematic);
}

void RigidController::set_mass(const float& mass) {
    m_dynamic->get()->setMass(mass);
}

void RigidController::set_angular_damping(const float& angDamp) {
    m_dynamic->get()->setAngularDamping(angDamp);
}

void RigidController::set_linear_damping(const float& linDamp) {
    m_dynamic->get()->setLinearDamping(linDamp);
}

void RigidController::set_angular_velocity(const vec3& angVel) {
    m_dynamic->get()->setAngularVelocity(RigidBody::vec3_to_PxVec3(angVel));
}

void RigidController::set_linear_velocity(const vec3& linVel) {
    m_dynamic->get()->setLinearVelocity(RigidBody::vec3_to_PxVec3(linVel));
}

void RigidController::add_force(const vec3& force) {
    m_dynamic->get()->addForce(RigidBody::vec3_to_PxVec3(force));
}

void RigidController::add_torque(const vec3& torque) {
    m_dynamic->get()->addTorque(RigidBody::vec3_to_PxVec3(torque));
}

void RigidController::clear_force() {
    m_dynamic->get()->clearForce();
}

void RigidController::clear_torque() {
    m_dynamic->get()->clearTorque();
}

void RigidController::putToSleep() {
    m_dynamic->get()->putToSleep();
}

void RigidController::wakeUp() {
    m_dynamic->get()->wakeUp();
}

}
}  // namespace components
}  // namespace knot
