#include <knoting/engine.h>
#include <knoting/rigidbody.h>
#include <knoting/scene.h>

namespace knot {
namespace components {
RigidBody::RigidBody()
    : m_physics(nullptr), m_dynamic(nullptr), m_static(nullptr), m_scene(nullptr), m_PxMaterial(nullptr) {}

RigidBody::~RigidBody() {}

void RigidBody::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_physics = engine.get_physics_module().lock()->get_physics().lock();
        m_scene = engine.get_physics_module().lock()->get_active_Scene().lock();
    }
}
void RigidBody::on_destroy() {
    if (m_dynamic) {
        m_scene->get()->removeActor(*m_dynamic->get());

    } else if (m_static) {
        m_scene->get()->removeActor(*m_static->get());
    }
}

vec3 RigidBody::get_position() {
    if (m_dynamic) {
        return PxVec3_to_vec3(m_dynamic->get()->getGlobalPose().p);

    } else {
        return PxVec3_to_vec3(m_static->get()->getGlobalPose().p);
    }
    return vec3(0.0f);
}

quat RigidBody::get_rotation() {
    if (m_dynamic) {
        return PxQuat_to_quat(m_dynamic->get()->getGlobalPose().q);
    } else {
        return PxQuat_to_quat(m_static->get()->getGlobalPose().q);
    }
    return quat();
}

std::weak_ptr<PxMaterial_ptr_wrapper> RigidBody::get_px_material() {
    return m_PxMaterial;
}

bool RigidBody::get_isKinematic() {
    return m_isKinematic;
}

void RigidBody::set_transform(const vec3& position, const quat& rotation) {
    if (m_dynamic == nullptr && m_static == nullptr) {
        return;
    }
    if (m_dynamic) {
        m_dynamic->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position)));
        m_dynamic->get()->setGlobalPose(PxTransform(quat_to_PxQuat(rotation)));
    } else {
        m_static->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position)));
        m_static->get()->setGlobalPose(PxTransform(quat_to_PxQuat(rotation)));
    }
}

void RigidBody::set_position(const vec3& position) {
    if (m_dynamic) {
        m_dynamic->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position)));
    } else {
        m_static->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position)));
    }
}
void RigidBody::set_rotation(const quat& rotation) {
    if (m_dynamic) {
        m_dynamic->get()->setGlobalPose(PxTransform(quat_to_PxQuat(rotation)));
    } else {
        m_static->get()->setGlobalPose(PxTransform(quat_to_PxQuat(rotation)));
    }
}

void RigidBody::create_actor(PxShape* shape, bool isKinematic, bool isDynamic, const float& mass) {
    shape->setLocalPose(PxTransform(get_rotation_from_transform()));
    m_isKinematic = isKinematic;
    if (isDynamic) {
        m_dynamic = std::make_shared<PxDynamic_ptr_wrapper>(
            m_physics->get()->createRigidDynamic(PxTransform(get_position_from_transform())));
        m_dynamic->get()->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*m_dynamic->get(), mass);
        m_dynamic->get()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, isKinematic);
        m_scene->get()->addActor(*m_dynamic->get());

    } else {
        m_static = std::make_shared<PxStatic_ptr_wrapper>(
            m_physics->get()->createRigidStatic(PxTransform(get_position_from_transform())));
        m_static->get()->attachShape(*shape);
        m_scene->get()->addActor(*m_static->get());
    }
}

void RigidBody::create_cube_shape(const vec3& halfSize,
                                  bool isKinematic,
                                  bool isDynamic,
                                  PxMaterial* material,
                                  const float& mass) {
    PxShape* shape = nullptr;
    if (m_PxMaterial) {
        if (material) {
            m_PxMaterial->set(material);
            shape = m_physics->get()->createShape(PxBoxGeometry(vec3_to_PxVec3(halfSize)), *m_PxMaterial->get());
        } else {
            shape = m_physics->get()->createShape(PxBoxGeometry(vec3_to_PxVec3(halfSize)), *m_PxMaterial->get());
        }

    } else {
        if (material) {
            m_PxMaterial = std::make_shared<PxMaterial_ptr_wrapper>(material);
            shape = m_physics->get()->createShape(PxBoxGeometry(vec3_to_PxVec3(halfSize)), *m_PxMaterial->get());
        } else {
            constexpr PxReal staticFriction = 0.3f;
            constexpr PxReal dynamicFriction = 0.3f;
            constexpr PxReal restitution = 0.6f;
            m_PxMaterial = std::make_shared<PxMaterial_ptr_wrapper>(
                m_physics->get()->createMaterial(staticFriction, dynamicFriction, restitution));
            shape = m_physics->get()->createShape(PxBoxGeometry(vec3_to_PxVec3(halfSize)), *m_PxMaterial->get());
        }
    }
    create_actor(shape, isKinematic, isDynamic, mass);
    shape->release();
}

void RigidBody::create_capsule_shape(const float& radius,
                                     const float& halfheight,
                                     bool isKinematic,
                                     bool isDynamic,
                                     PxMaterial* material,
                                     const float& mass) {
    PxShape* shape = nullptr;
    if (m_PxMaterial) {
        if (material) {
            m_PxMaterial->set(material);
            shape = m_physics->get()->createShape(PxCapsuleGeometry(radius, halfheight), *m_PxMaterial->get());
        } else {
            shape = m_physics->get()->createShape(PxCapsuleGeometry(radius, halfheight), *m_PxMaterial->get());
        }

    } else {
        if (material) {
            m_PxMaterial->set(material);
            shape = m_physics->get()->createShape(PxCapsuleGeometry(radius, halfheight), *m_PxMaterial->get());
        } else {
            constexpr PxReal staticFriction = 0.3f;
            constexpr PxReal dynamicFriction = 0.3f;
            constexpr PxReal restitution = 0.3f;
            m_PxMaterial = std::make_shared<PxMaterial_ptr_wrapper>(
                m_physics->get()->createMaterial(staticFriction, dynamicFriction, restitution));
            shape = m_physics->get()->createShape(PxCapsuleGeometry(radius, halfheight), *m_PxMaterial->get());
        }
    }
    create_actor(shape, isKinematic, isDynamic, mass);
    shape->release();
}

void RigidBody::create_sphere_shape(const float& radius,
                                    bool isKinematic,
                                    bool isDynamic,
                                    PxMaterial* material,
                                    const float& mass) {
    PxShape* shape = nullptr;
    if (m_PxMaterial) {
        if (material) {
            m_PxMaterial->set(material);
            shape = m_physics->get()->createShape(PxSphereGeometry(radius), *m_PxMaterial->get());
        } else {
            shape = m_physics->get()->createShape(PxSphereGeometry(radius), *m_PxMaterial->get());
        }

    } else {
        if (material) {
            m_PxMaterial->set(material);
            shape = m_physics->get()->createShape(PxSphereGeometry(radius), *m_PxMaterial->get());
        } else {
            constexpr PxReal staticFriction = 0.3f;
            constexpr PxReal dynamicFriction = 0.3f;
            constexpr PxReal restitution = 0.3f;
            m_PxMaterial = std::make_shared<PxMaterial_ptr_wrapper>(
                m_physics->get()->createMaterial(staticFriction, dynamicFriction, restitution));
            shape = m_physics->get()->createShape(PxSphereGeometry(radius), *m_PxMaterial->get());
        }
    }
    create_actor(shape, isKinematic, isDynamic, mass);
    shape->release();
}

PxVec3 RigidBody::get_position_from_transform() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        ;
        if (goOpt) {
            PxVec3 position = vec3_to_PxVec3(goOpt->get_component<components::Transform>().get_position());

            return position;
        }
    }
    return PxVec3(std::numeric_limits<float>::max());
}

PxQuat RigidBody::get_rotation_from_transform() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            return quat_to_PxQuat(goOpt->get_component<components::Transform>().get_rotation());
        }
    }
    return PxQuat();
}

vec3 RigidBody::PxVec3_to_vec3(PxVec3 v) {
    return vec3(v.x, v.y, v.z);
}

PxVec3 RigidBody::vec3_to_PxVec3(vec3 v) {
    return PxVec3(v.x, v.y, v.z);
}

quat RigidBody::PxQuat_to_quat(PxQuat q) {
    return quat(q.w, q.x, q.y, q.z);
}

PxQuat RigidBody::quat_to_PxQuat(quat q) {
    return PxQuat(q.x, q.y, q.z, q.w);
}

}  // namespace components
}  // namespace knot
