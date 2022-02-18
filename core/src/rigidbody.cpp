#include <knoting/rigidbody.h>

namespace knot {
namespace components {
RigidBody::RigidBody() : m_physics(NULL), m_dynamic(NULL), m_static(NULL), m_scene(NULL), m_material(NULL){
    m_transform = physx::PxTransform(PxVec3(0, 0, 0));

}

RigidBody::~RigidBody() {
    m_scene = NULL;
    m_physics = NULL;

}

void RigidBody::on_awake() {}
void RigidBody::on_destroy() {}

vec3 RigidBody::get_position() const {
    vec3 v;
    if (m_dynamic) {
        v = vec3(m_dynamic->getGlobalPose().p.x, m_dynamic->getGlobalPose().p.y, m_dynamic->getGlobalPose().p.z);

    } else {
        v = vec3(m_static->getGlobalPose().p.x, m_static->getGlobalPose().p.y, m_static->getGlobalPose().p.z);
    }
    return v;
}

quat RigidBody::get_rotation() const {
    quat q;
    if (m_dynamic) {
        q = quat(m_dynamic->getGlobalPose().q.w, m_dynamic->getGlobalPose().q.x, m_dynamic->getGlobalPose().q.y,
             m_dynamic->getGlobalPose().q.z);
    }else{
        q = quat(m_static->getGlobalPose().q.w, m_static->getGlobalPose().q.x, m_static->getGlobalPose().q.y,
                 m_static->getGlobalPose().q.z);

    }
    return q;
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
    physx::PxShape* shape = NULL;
    if (m_material) {
        shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    } else {
        m_material = m_physics->createMaterial(0.3f, 0.3f, 0.6f);
        shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    }
    m_dynamic = m_physics->createRigidDynamic(PxTransform(m_transform.p));
    m_dynamic->attachShape(*shape);
    PxRigidBodyExt::updateMassAndInertia(*m_dynamic, mass);
    m_scene->addActor(*m_dynamic);
    shape->release();
}

void RigidBody::create_cube_rigid_static(const vec3& size) {
    physx::PxShape* shape = NULL;
    if (m_material) {
        shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    } else {
        m_material = m_physics->createMaterial(0.3f, 0.3f, 0.6f);
        shape = m_physics->createShape(physx::PxBoxGeometry(size.x, size.y, size.z), *m_material);
    }
    m_static = m_physics->createRigidStatic(PxTransform(m_transform.p));
    m_static->attachShape(*shape);
    m_scene->addActor(*m_static);
    shape->release();
}

}  // namespace components
}  // namespace knot
