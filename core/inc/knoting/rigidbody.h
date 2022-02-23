#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>
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

    vec3 get_position();
    quat get_rotation();
    std::weak_ptr<PxMaterial_ptr_wrapper> get_px_material();
    bool get_isKinematic();

    // could use after creat rigid
    void set_transform(const vec3& position, const quat& rotation = quat());
    void set_position(const vec3& position);
    void set_rotation(const quat& rotation);
    void set_kinematic(bool isKinematic);
    //

    void create_actor(PxShape* shape, bool isKinematic, bool isDynamic, const float& mass = 0);

    void create_cube_shape(const vec3& halfSize,
                           bool isKinematic,
                           bool isDynamic,
                           PxMaterial* material = nullptr,
                           const float& mass = 0);

    void create_capsule_shape(const float& radius,
                              const float& halfheight,
                              bool isKinematic,
                              bool isDynamic,
                              PxMaterial* material = nullptr,
                              const float& mass = 0);

    void create_sphere_shape(const float& radius,
                             bool isKinematic,
                             bool isDynamic,
                             PxMaterial* material = nullptr,
                             const float& mass = 0);

    PxVec3 get_position_from_transform();
    PxQuat get_rotation_from_transform();

    vec3 PxVec3_to_vec3(PxVec3 v);
    PxVec3 vec3_to_PxVec3(vec3 v);
    quat PxQuat_to_quat(PxQuat q);
    PxQuat quat_to_PxQuat(quat q);

   protected:
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
    std::shared_ptr<PxDynamic_ptr_wrapper> m_dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> m_static;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<PxMaterial_ptr_wrapper> m_PxMaterial;
    bool m_isKinematic;
};

}  // namespace components
}  // namespace knot
