#pragma once
#include <knoting/components.h>
#include <knoting/px_variables_wrapper.h>

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
        if (m_ishit) {
            return m_maxDistance;
        }
        return std::numeric_limits<float>::max();
    }
    bool get_ishit() { return m_ishit; }
    vec3 get_hitPosition();
    vec3 get_hitNomal();
    float get_hitDistance();
    std::weak_ptr<PxShape_ptr_wrapper> get_hitShape();

    void set_origin(vec3 origin);
    void set_unitDir(vec3 unitDir);
    void set_maxDistance(float maxDistance);

   protected:
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    PxVec3 m_origin;
    PxVec3 m_unitDir;
    PxReal m_maxDistance;
    bool m_ishit;
    PxRaycastBuffer m_hit;
};

}  // namespace components
}  // namespace knot
