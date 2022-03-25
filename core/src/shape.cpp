#include <knoting/engine.h>
#include <knoting/shape.h>
#include <cereal/archives/json.hpp>

namespace knot {
namespace components {
Shape::Shape() : m_material(nullptr), m_shape(nullptr) {}
Shape::~Shape() {}

void Shape::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_physics = engine.get_physics_module().lock()->get_physics().lock();
    }
    if (get_PxMaterial_from_pxmaterial()) {
        m_material = get_PxMaterial_from_pxmaterial();
    } else {
        constexpr PxReal default_staticFriction = 0.3f;
        constexpr PxReal default_dynamicFriction = 0.3f;
        constexpr PxReal default_restitution = 0.6f;
        m_material = std::make_shared<PxMaterial_ptr_wrapper>(
            m_physics->get()->createMaterial(default_staticFriction, default_dynamicFriction, default_restitution));
    }
}

void Shape::on_destroy() {}

void Shape::set_geometry(const PxGeometry& geometry) {
    if (!m_shape) {
        m_shape = std::make_shared<PxShape_ptr_wrapper>(m_physics->get()->createShape(geometry, *m_material->get()));
        PxFilterData data;
        set_filter_data(filter_group::eAll, filter_group::eAll | filter_group::eParticle_red |
                                                filter_group::ePlayer_blue | filter_group::ePlayer_red |
                                                filter_group::ePlayer_blue);

    } else {
        m_shape->get()->setGeometry(geometry);
        set_filter_data(filter_group::eAll, filter_group::eAll | filter_group::eParticle_red |
                                                filter_group::ePlayer_blue | filter_group::ePlayer_red |
                                                filter_group::ePlayer_blue);
    }
}

void Shape::set_local_rotation(quat rotation) {
    if (m_shape) {
        m_shape->get()->setLocalPose(PxTransform(RigidBody::quat_to_PxQuat(rotation)));
    }
}

void Shape::set_flag(PxShapeFlag::Enum flag) {
    m_shape->get()->setFlag(flag, true);
}

void Shape::remove_flag(PxShapeFlag::Enum flag) {
    m_shape->get()->setFlag(flag, false);
}

void Shape::set_filter_data(PxU32 group, PxU32 mask) {
    PxFilterData data;
    data.word0 = group;
    data.word1 = mask;
    m_shape->get()->setSimulationFilterData(data);
}

PxBoxGeometry Shape::create_cube_geometry(const vec3& halfsize) {
    return PxBoxGeometry(RigidBody::vec3_to_PxVec3(halfsize));
}

PxSphereGeometry Shape::create_sphere_geometry(const float& radius) {
    return PxSphereGeometry(radius);
}

PxCapsuleGeometry Shape::create_capsule_geometry(const float& radius, const float& halfheight) {
    return PxCapsuleGeometry(radius, halfheight);
}

std::shared_ptr<PxMaterial_ptr_wrapper> Shape::get_PxMaterial_from_pxmaterial() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            if (goOpt->has_component<components::PhysicsMaterial>()) {
                PhysicsMaterial& pm = goOpt->get_component<components::PhysicsMaterial>();
                return pm.get_px_material().lock();
            }
        }
    }
    return nullptr;
}

template <class Archive>
void Shape::save(Archive& archive) const {
    // vec3 of the half size/ radius/ radius and half height
    //  a reference to the type of geometry
    vec3 shapeSize;
    PxGeometryType::Enum shapeType = m_shape->get()->getGeometry().getType();
    switch (shapeType) {
        case PxGeometryType::Enum::eBOX: {
            PxBoxGeometry cubeGeometry;
            m_shape->get()->getBoxGeometry(cubeGeometry);
            shapeSize = RigidBody::PxVec3_to_vec3(cubeGeometry.halfExtents);
            break;
        }
        case PxGeometryType::Enum::eSPHERE: {
            PxSphereGeometry sphereGeometry;
            m_shape->get()->getSphereGeometry(sphereGeometry);
            shapeSize = vec3(sphereGeometry.radius, 0, 0);
            break;
        }
        case PxGeometryType::Enum::eCAPSULE: {
            PxCapsuleGeometry capsuleGeometry;
            m_shape->get()->getCapsuleGeometry(capsuleGeometry);
            shapeSize = vec3(capsuleGeometry.radius, capsuleGeometry.halfHeight, 0);
            break;
        }
        default:
            log::debug("Shape Save: Geometry Type not Found");
            break;
    }
    archive(CEREAL_NVP(shapeType), CEREAL_NVP(shapeSize));
}

template <class Archive>
void Shape::load(Archive& archive) {
    vec3 shapeSize;
    PxGeometryType::Enum shapeType;
    archive(CEREAL_NVP(shapeType), CEREAL_NVP(shapeSize));
    m_shapeType = shapeType;
    m_shapeSize = shapeSize;
}

void Shape::on_load() {
    on_awake();
    switch (m_shapeType) {
        case PxGeometryType::Enum::eBOX: {
            this->set_geometry(this->create_cube_geometry(m_shapeSize));
            break;
        }
        case PxGeometryType::Enum::eSPHERE: {
            this->set_geometry(this->create_sphere_geometry(m_shapeSize.x));
            break;
        }
        case PxGeometryType::Enum::eCAPSULE: {
            this->set_geometry(this->create_capsule_geometry(m_shapeSize.x, m_shapeSize.y));
            break;
        }
        default:
            log::debug("Shape Load: Geometry Type not Found");
            break;
    }
}
void Shape::set_local_position(vec3 position) {
    posOffset = position;
    if (m_shape) {
        m_shape->get()->setLocalPose(PxTransform(RigidBody::vec3_to_PxVec3(posOffset)));
    }
}

template void Shape::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
template void Shape::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);

}  // namespace components
}  // namespace knot
