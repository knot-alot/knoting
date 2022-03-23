#pragma once
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>

namespace knot {
namespace components {
class RigidController {
   public:
    RigidController();
    ~RigidController();

    void on_awake();
    void on_destroy();
    void on_load();

    std::weak_ptr<PxDynamic_ptr_wrapper> get_dynamic();
    bool get_is_kinematic();
    bool get_is_awake();
    float get_mass();
    float get_angular_damping();
    float get_linear_damping();
    vec3 get_angular_velocity();
    vec3 get_linear_velocity();

    void set_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> dynamic);
    void set_kinematic(const bool& isKinematic);
    void set_mass(const float& mass);
    void set_angular_damping(const float& angDamp);
    void set_linear_damping(const float& linDamp);
    void set_angular_velocity(const vec3& angVel);
    void set_linear_velocity(const vec3& linVel);
    void add_force(const vec3& force);
    void add_torque(const vec3& torque);
    void clear_force();
    void clear_torque();

    void lockRotations(bool x = true, bool y = true, bool z = true);

    void put_to_sleep();
    void wake_up();

    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_isAwake), CEREAL_NVP(m_isKinematic));
    }

    template <class Archive>
    void load(Archive& archive) {
        bool isAwake, isKinematic;
        archive(CEREAL_NVP(isAwake), CEREAL_NVP(isKinematic));
        on_awake();
        m_isAwake = isAwake;
        m_isKinematic = isKinematic;
    }

   protected:
    std::shared_ptr<PxDynamic_ptr_wrapper> m_dynamic;
    bool m_isAwake;
    bool m_isKinematic;
};

}  // namespace components
}  // namespace knot
