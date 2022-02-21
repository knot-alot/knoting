#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/types.h>

using namespace physx;

namespace knot {
namespace components {

class RigidBody {
    // fllow this way
    // set physics and scene
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

    //could use after creat rigid
    void set_transform(const vec3& position, const quat& rotation = quat());
    void set_position(const vec3& position);
    void set_rotation(const quat& rotation);


    void set_material(physx::PxMaterial* material);
    void set_physics_and_scene(physx::PxScene* scene, physx::PxPhysics* physics);

    void create_cube_rigid_dynamic(const vec3& halfSize,
                                   const float& mass,
                                   const vec3& position,
                                   const quat& rotation = quat());
    void create_cube_rigid_static( const vec3& size,
                                   const vec3& position,
                                   const quat& rotation = quat());
    void create_capsule_rigid_dynamic(const float& radius,
                                      const float& halfheight,
                                      const float& mass,
                                      const vec3& position,
                                      const quat& rotation = quat());
    void create_capsule_rigid_static(const float& radius,
                                     const float& halfheight,
                                     const vec3& position,
                                     const quat& rotation = quat());
    void create_sphere_rigid_dynamic(const float& radius,
                                     const float& mass,
                                     const vec3& position,
                                     const quat& rotation = quat());
    void create_sphere_rigid_static(const float& radius,
                                    const vec3& position,
                                    const quat& rotation = quat());

   protected:
    physx::PxPhysics* m_physics;
    physx::PxRigidDynamic* m_dynamic;
    physx::PxRigidStatic* m_static;
    physx::PxScene* m_scene;
    physx::PxMaterial* m_material;
};

}  // namespace components
}  // namespace knot
