#include <knoting/engine.h>
#include <knoting/rigidbody.h>
#include <knoting/scene.h>

namespace knot {
namespace components {
RigidBody::RigidBody()
    : m_physics(nullptr), m_dynamic(nullptr), m_static(nullptr), m_scene(nullptr), m_shape(nullptr) {}

RigidBody::~RigidBody() {}

void RigidBody::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_physics = engine.get_physics_module().lock()->get_physics().lock();
        m_scene = engine.get_physics_module().lock()->get_active_Scene().lock();
    }
    if (get_shape_from_shape()) {
        m_shape = get_shape_from_shape();
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

std::weak_ptr<PxDynamic_ptr_wrapper> RigidBody::get_dynamic() {
    return m_dynamic;
}

std::weak_ptr<PxStatic_ptr_wrapper> RigidBody::get_static() {
    return m_static;
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

void RigidBody::create_actor(bool isDynamic, const float& mass) {
    if (isDynamic) {
        m_dynamic = std::make_shared<PxDynamic_ptr_wrapper>(
            m_physics->get()->createRigidDynamic(PxTransform(get_position_from_transform())));
        m_dynamic->get()->attachShape(*m_shape->get());
        PxRigidBodyExt::updateMassAndInertia(*m_dynamic->get(), mass);
        m_scene->get()->addActor(*m_dynamic->get());

    } else {
        m_static = std::make_shared<PxStatic_ptr_wrapper>(
            m_physics->get()->createRigidStatic(PxTransform(get_position_from_transform())));
        m_static->get()->attachShape(*m_shape->get());
        m_scene->get()->addActor(*m_static->get());
    }
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

std::shared_ptr<PxShape_ptr_wrapper> RigidBody::get_shape_from_shape() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            if (goOpt->has_component<components::Shape>()) {
                Shape& shape = goOpt->get_component<components::Shape>();
                return shape.get_shape().lock();
            }
        }
    }
    return nullptr;
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
