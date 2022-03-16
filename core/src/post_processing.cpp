#include <knoting/post_processing.h>

namespace knot {
namespace components {

PostProcessing::PostProcessing() {}

PostProcessing::~PostProcessing() {}

void PostProcessing::on_awake() {
    m_shader.load_shader("postprocess", "vs_postProcess.bin", "fs_postProcess.bin");

    m_uniformGeometry = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    m_uniformGui = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
}

void PostProcessing::on_destroy() {}

void PostProcessing::set_uniforms() {
    if (isValid(m_geoTBH)) {
        bgfx::setTexture(0, m_uniformGeometry, m_geoTBH);
    }
    if (isValid(m_guiTBH)) {
        bgfx::setTexture(1, m_uniformGui, m_guiTBH);
    }
}

void PostProcessing::set_color_render_texture(const bgfx::TextureHandle& tbh) {
    m_geoTBH = tbh;
}

void PostProcessing::set_gui_render_texture(const bgfx::TextureHandle& tbh) {
    m_guiTBH = tbh;
}

}  // namespace components
}  // namespace knot