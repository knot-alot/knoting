#pragma once
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/types.h>

namespace knot {
namespace components {
class Raycast {
   public:
    Raycast();
    ~Raycast();

    void on_awake();
    void on_destroy();

    void raycast();

    vec3 get_origin();
    vec3 get_unitDir();
    float get_maxDistance() {
        if (m_isHit) {
            return m_maxDistance;
        }
        return std::numeric_limits<float>::max();
    }
    bool get_is_hit() { return m_isHit; }
    vec3 get_hit_position();
    vec3 get_hit_normal();
    float get_hit_distance();
    std::weak_ptr<PxShape_ptr_wrapper> get_hit_shape();

    void set_origin(const vec3& origin);
    void set_unit_dir(const vec3& unitDir);
    void set_max_distance(const float& maxDistance);
    void set_raycast(const vec3& origin, const vec3& unitDir, const float& maxDistance);

    template <class Archive>
    void save(Archive& archive) const;

    template <class Archive>
    void load(Archive& archive);

   private:
    PxVec3 get_position_from_transform();

   protected:
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    PxVec3 m_origin;
    PxVec3 m_unitDir;
    PxReal m_maxDistance;
    bool m_isHit;
    PxRaycastBuffer m_hit;
};

}  // namespace components
}  // namespace knot
