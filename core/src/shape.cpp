#include <knoting/engine.h>
#include <knoting/shape.h>

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
        constexpr PxReal defalut_staticFriction = 0.3f;
        constexpr PxReal defalut_dynamicFriction = 0.3f;
        constexpr PxReal defalut_restitution = 0.6f;
        m_material = std::make_shared<PxMaterial_ptr_wrapper>(
            m_physics->get()->createMaterial(defalut_staticFriction, defalut_dynamicFriction, defalut_restitution));
    }
}

void Shape::on_destroy() {}

void Shape::set_geometry(PxGeometry& geometry) {
    if (!m_shape) {
        m_shape = std::make_shared<PxShape_ptr_wrapper>(m_physics->get()->createShape(geometry, *m_material->get()));

    } else {
        m_shape->get()->setGeometry(geometry);
    }
}

void Shape::set_local_rotation(quat rotation) {
    if (m_shape) {
        m_shape->get()->setLocalPose(PxTransform(RigidBody::quat_to_PxQuat(rotation)));
    }
}

PxBoxGeometry& Shape::create_cube_geometry(const vec3& halfsize) {
    return PxBoxGeometry(RigidBody::vec3_to_PxVec3(halfsize));
}

PxSphereGeometry& Shape::create_sphere_geometry(const float& radius) {
    return PxSphereGeometry(radius);
}

PxCapsuleGeometry& Shape::create_capsule_geometry(const float& radius, const float& halfheight) {
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

}  // namespace components
}  // namespace knot
