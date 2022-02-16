#include <knoting/rigidbody.h>

namespace knot {
namespace components {
RigidBody::RigidBody() : m_physics(NULL), m_dynamic(NULL), m_static(NULL), m_scene(NULL), m_material(NULL) {
    m_transform = physx::PxTransform(0, 0, 0);
}

RigidBody::~RigidBody() {
    m_material->release();
    m_material = NULL;
    m_dynamic->release();
    m_dynamic = NULL;
    m_static->release();
    m_static = NULL;
    m_physics->release();
    m_physics = NULL;
    m_scene->release();
    m_scene = NULL;
}

void RigidBody::on_awake() {}
void RigidBody::on_destroy() {}

vec3 RigidBody::get_position() const {
    return vec3(m_transform.p.x, m_transform.p.y, m_transform.p.z);
}

quat RigidBody::get_rotation() const {
    return quat(m_transform.q.w, m_transform.q.x, m_transform.q.y, m_transform.q.z);
}

physx::PxMaterial* RigidBody::get_pxmaterial() {
    return m_material;
}

void RigidBody::set_transform(const vec3& position, const quat& rotation) {
    float p_x = position.x;
    float p_y = position.y;
    float p_z = position.z;
    float r_w = rotation.w;
    float r_x = rotation.x;
    float r_y = rotation.y;
    float r_z = rotation.z;
    m_transform = physx::PxTransform(physx::PxVec3(p_x, p_y, p_z), physx::PxQuat(r_w, r_x, r_y, r_z));
}

void RigidBody::set_material(physx::PxMaterial* material) {
    m_material = material;
}

void RigidBody::set_physics_and_scene(physx::PxScene* scene, physx::PxPhysics* physics) {
    m_scene = scene;
    m_physics = physics;
}

void RigidBody::create_cube_rigid_dynamic(const vec3& size, const float& mass) {
    if (m_material) {
        physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    } else {
        m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);
        physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    }
    physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    m_dynamic = m_physics->createRigidDynamic(m_transform);
    m_dynamic->attachShape(*shape);
    PxRigidBodyExt::updateMassAndInertia(*m_dynamic, mass);
    m_scene->addActor(*m_dynamic);
    shape->release();
}

void RigidBody::create_cube_rigid_static(const vec3& size) {
    if (m_material) {
        physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    } else {
        m_material = m_physics->createMaterial(0.5f, 0.5f, 0.6f);
        physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    }
    physx::PxShape* shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    m_static = m_physics->createRigidStatic(m_transform);
    m_static->attachShape(*shape);
    m_scene->addActor(*m_static);
    shape->release();
}

}  // namespace components
}  // namespace knot
