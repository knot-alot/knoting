#include <knoting/aggregate.h>
#include <knoting/engine.h>
#include <knoting/rigidbody.h>
#include <knoting/scene.h>

namespace knot {
namespace components {
RigidBody::RigidBody()
    : m_physics(nullptr),
      m_dynamic(nullptr),
      m_static(nullptr),
      m_scene(nullptr),
      m_shape(nullptr),
      m_aggregate(nullptr) {}

RigidBody::~RigidBody() {}

void RigidBody::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        this->m_physics = engine.get_physics_module().lock()->get_physics().lock();
        this->m_scene = engine.get_physics_module().lock()->get_active_Scene().lock();
    }
    this->m_shape = get_shape_from_shape();
    this->m_aggregate = get_aggregate_from_aggregate();
}
void RigidBody::on_destroy() {
    if (m_dynamic) {
        m_scene->get()->removeActor(*m_dynamic->get());
        delete reinterpret_cast<uuid*>(m_dynamic.get()->get()->userData);
    } else if (m_static) {
        m_scene->get()->removeActor(*m_static->get());
        delete reinterpret_cast<uuid*>(m_static.get()->get()->userData);
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
        m_dynamic->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position), quat_to_PxQuat(rotation)));
    } else {
        m_static->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position), quat_to_PxQuat(rotation)));
    }
}

void RigidBody::set_position(const vec3& position) {
    if (m_dynamic) {
        m_dynamic->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position), m_dynamic->get()->getGlobalPose().q));
    } else {
        m_static->get()->setGlobalPose(PxTransform(vec3_to_PxVec3(position), m_static->get()->getGlobalPose().q));
    }
}

void RigidBody::set_rotation(const quat& rotation) {
    if (m_dynamic) {
        m_dynamic->get()->setGlobalPose(PxTransform(m_dynamic->get()->getGlobalPose().p, quat_to_PxQuat(rotation)));
    } else {
        m_static->get()->setGlobalPose(PxTransform(m_static->get()->getGlobalPose().p, quat_to_PxQuat(rotation)));
    }
}

void RigidBody::set_name(const std::string& name) {
    m_name = name;
    if (m_dynamic) {
        m_dynamic->get()->setName(name.c_str());
    } else {
        m_static->get()->setName(name.c_str());
    }
}

void RigidBody::set_flag(PxActorFlag::Enum flag) {
    if (m_dynamic) {
        m_dynamic->get()->setActorFlag(flag, true);
    } else {
        m_static->get()->setActorFlag(flag, true);
    }
}

void RigidBody::remove_flag(PxActorFlag::Enum flag) {
    if (m_dynamic) {
        m_dynamic->get()->setActorFlag(flag, false);
    } else {
        m_static->get()->setActorFlag(flag, false);
    }
}

void RigidBody::set_shape(std::shared_ptr<PxShape_ptr_wrapper> shape) {
    if (m_dynamic) {
        if (m_shape) {
            m_dynamic->get()->detachShape(*m_shape->get());
            m_shape = shape;
            m_dynamic->get()->attachShape(*m_shape->get());
        } else {
            m_shape = shape;
        }
    } else {
        if (m_shape) {
            m_static->get()->detachShape(*m_shape->get());
            m_shape = shape;
            m_static->get()->attachShape(*m_shape->get());
        } else {
            m_shape = shape;
        }
    }
}

void RigidBody::set_aggregate(std::shared_ptr<PxAggregate_ptr_wrapper> aggragate) {
    if (m_dynamic) {
        if (m_aggregate) {
            m_aggregate->get_aggregate()->removeActor(*m_dynamic->get());
            m_aggregate = aggragate;
            m_aggregate->get_aggregate()->addActor(*m_dynamic->get());
        } else {
            m_aggregate = aggragate;
        }
    } else {
        if (m_aggregate) {
            m_aggregate->get_aggregate()->removeActor(*m_static->get());
            m_aggregate = aggragate;
            m_aggregate->get_aggregate()->addActor(*m_static->get());
        } else {
            m_aggregate = aggragate;
        }
    }
}

void RigidBody::create_actor(bool isDynamic, const float& mass) {
    auto goOpt = GameObject::get_game_object_from_component(*this);

    if (isDynamic) {
        m_dynamic = std::make_shared<PxDynamic_ptr_wrapper>(m_physics->get()->createRigidDynamic(
            PxTransform(get_position_from_transform(), get_rotation_from_transform())));
        m_dynamic->get()->attachShape(*m_shape->get());
        PxRigidBodyExt::updateMassAndInertia(*m_dynamic->get(), mass);
        if(m_aggregate){
            m_aggregate->get_aggregate()->addActor(*m_dynamic->get());
        }else{
            m_scene->get()->addActor(*m_dynamic->get());
        }


        if (goOpt)
            m_dynamic.get()->get()->userData = new uuid(goOpt->get_id());

        m_isDynamic = isDynamic;

        PhysicsAbstract abstract;
        abstract.dynamic = m_dynamic;
        Physics::uuidToAbstract[goOpt->get_id()] = abstract;
    } else {
        m_static = std::make_shared<PxStatic_ptr_wrapper>(m_physics->get()->createRigidStatic(
            PxTransform(get_position_from_transform(), get_rotation_from_transform())));
        m_static->get()->attachShape(*m_shape->get());
        if(m_aggregate){
            m_aggregate->get_aggregate()->addActor(*m_static->get());
        }else{
            m_scene->get()->addActor(*m_static->get());
        }

        if (goOpt)
            m_static.get()->get()->userData = new uuid(goOpt->get_id());

        m_isDynamic = isDynamic;

        PhysicsAbstract abstract;
        abstract.statik = m_static;
        Physics::uuidToAbstract[goOpt->get_id()] = abstract;
    }
}

PxActorFlags RigidBody::get_flags() {
    if (m_dynamic) {
        return m_dynamic->get()->getActorFlags();
    } else {
        return m_static->get()->getActorFlags();
    }
}

PxVec3 RigidBody::get_position_from_transform() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        auto goOpt = GameObject::get_game_object_from_component(*this);
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
        auto goOpt = GameObject::get_game_object_from_component(*this);
        if (goOpt) {
            Transform& transform = goOpt->get_component<Transform>();
            quat r = transform.get_rotation();
            return quat_to_PxQuat(r);
        }
    }
    return PxQuat();
}

std::shared_ptr<PxShape_ptr_wrapper> RigidBody::get_shape_from_shape() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return nullptr;
    }
    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (!goOpt) {
        return nullptr;
    }
    if (!goOpt->has_component<components::Shape>()) {
        return nullptr;
    }
    Shape& shape = goOpt->get_component<components::Shape>();
    return shape.get_shape().lock();
}

std::shared_ptr<PxAggregate_ptr_wrapper> RigidBody::get_aggregate_from_aggregate() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return nullptr;
    }
    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (!goOpt) {
        return nullptr;
    }
    if (!goOpt->has_component<components::Aggregate>()) {
        return nullptr;
    }
    Aggregate& aggregate = goOpt->get_component<components::Aggregate>();
    return aggregate.get_aggregate().lock();
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
void RigidBody::on_load() {
    this->on_awake();
    this->create_actor(m_isDynamic, m_mass);
}

}  // namespace components
}  // namespace knot
