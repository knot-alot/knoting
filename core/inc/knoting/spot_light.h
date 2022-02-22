#pragma once
#include <knoting/types.h>

namespace knot {
namespace components {
class SpotLight {
   public:
    SpotLight();
    ~SpotLight();

    //=For ECS========
    void on_awake();
    void on_destroy();
    //================

    float get_outer_radius() { return m_lightOuterRadius; };
    float get_inner_radius() { return m_lightInnerRadius; };
    vec3 get_color() { return m_color; };

    void set_outer_radius(float lightOuterRadius) { m_lightOuterRadius = lightOuterRadius; };
    void set_inner_radius(float lightInnerRadius) { m_lightInnerRadius = lightInnerRadius; };
    void set_color(vec3 color) { m_color = color; };

    template <class Archive>
    void serialize(Archive &archive){
        archive(CEREAL_NVP(m_lightOuterRadius),CEREAL_NVP(m_lightInnerRadius),CEREAL_NVP(m_color));
    }

   private:
    float m_lightOuterRadius = 1.0f;
    float m_lightInnerRadius = 0.5f;
    vec3 m_color = vec3(3.0f);
};
}  // namespace components
}  // namespace knot
