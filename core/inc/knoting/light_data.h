#pragma once
#include <bgfx/bgfx.h>
#include <knoting/types.h>

namespace knot {

class SpotlightData {
   public:
    SpotlightData();
    ~SpotlightData();

   public:
    bgfx::UniformHandle u_spotlightPosRadius;
    bgfx::UniformHandle u_spotlightRgbInnerR;

    uint16_t m_amountOfSpotlights = 0;
    std::vector<vec4> m_spotlightsPositionOuterRadius;
    std::vector<vec4> m_spotlightsColorInnerRadius;
};

class LightData {
   public:
    LightData();

    void push_spotlight_pos_outer_rad(vec4 posOutRad);
    void push_spotlight_color_inner_rad(vec4 colInRad);
    void set_spotlight_count(uint16_t count);
    void set_spotlight_uniforms();
    void clear_spotlight();

    SpotlightData m_spotlightData;
};

}  // namespace knot
