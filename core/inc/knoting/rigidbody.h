#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/types.h>

using namespace physx;

namespace knot {
namespace components {

class RigidBody {
    // fllow this way
    // set physics and scene
    // set transform
    // set material(already defalut)
    // creat rigidbody

   public:
    RigidBody();
    ~RigidBody();

    void on_awake();
    void on_destroy();

    vec3 get_position() const;
    quat get_rotation() const;
    physx::PxMaterial* get_pxmaterial();

    void set_transform(const vec3& position, const quat& rotation = quat());
    void set_material(physx::PxMaterial* material);
    void set_physics_and_scene(physx::PxScene* scene, physx::PxPhysics* physics);
    void create_cube_rigid_dynamic(const vec3& scale, const float& mass);
    void create_cube_rigid_static(const vec3& scale);

   protected:
    physx::PxPhysics* m_physics;
    physx::PxTransform m_transform;
    physx::PxRigidDynamic* m_dynamic;
    physx::PxRigidStatic* m_static;
    physx::PxScene* m_scene;
    physx::PxMaterial* m_material;
};

}  // namespace components
}  // namespace knot
