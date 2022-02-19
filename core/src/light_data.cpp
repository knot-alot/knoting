#include <knoting/light_data.h>

namespace knot {

LightData::LightData() {}

void LightData::set_spotlight_uniforms() {
    bgfx::setUniform(m_spotlightData.u_spotlightPosRadius, m_spotlightData.m_spotlightsPositionOuterRadius.data(),
                     m_spotlightData.m_amountOfSpotlights);
    bgfx::setUniform(m_spotlightData.u_spotlightRgbInnerR, m_spotlightData.m_spotlightsColorInnerRadius.data(),
                     m_spotlightData.m_amountOfSpotlights);
}

void LightData::clear_spotlight() {
    m_spotlightData.m_spotlightsPositionOuterRadius.clear();
    m_spotlightData.m_spotlightsColorInnerRadius.clear();
}

void LightData::push_spotlight_pos_outer_rad(vec4 posOutRad) {
    m_spotlightData.m_spotlightsPositionOuterRadius.emplace_back(posOutRad);
}

void LightData::push_spotlight_color_inner_rad(vec4 colInRad) {
    m_spotlightData.m_spotlightsColorInnerRadius.emplace_back(colInRad);
}

void LightData::set_spotlight_count(uint16_t count) {
    m_spotlightData.m_amountOfSpotlights = count;
}

SpotlightData::SpotlightData() {
    // TODO replace "u_spotlightRgbInnerR" with "u_spotlightPosRadius" when pbr shader is impl
    // TODO replace "u_lightRgbInnerR" with "u_spotlightRgbInnerR" when pbr shader is impl
    u_spotlightPosRadius = bgfx::createUniform("u_lightPosRadius", bgfx::UniformType::Vec4);
    u_spotlightRgbInnerR = bgfx::createUniform("u_lightRgbInnerR", bgfx::UniformType::Vec4);
}

SpotlightData::~SpotlightData() {
    bgfx::destroy(u_spotlightPosRadius);
    bgfx::destroy(u_spotlightRgbInnerR);
}

}  // namespace knot
