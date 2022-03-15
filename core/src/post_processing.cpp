#include <knoting/post_processing.h>

namespace knot {
namespace components {

PostProcessing::PostProcessing() {}
PostProcessing::~PostProcessing() {}

void PostProcessing::set_geometry_framebuffer(const bgfx::FrameBufferHandle& fbh) {
    m_geometryPass = fbh;
}
void PostProcessing::set_gui_framebuffer(const bgfx::FrameBufferHandle& fbh) {
    m_guiPass = fbh;
}

uint16_t PostProcessing::get_geometry_id() {
    return m_geometryPass.idx;
}

uint16_t PostProcessing::get_gui_id() {
    return m_guiPass.idx;
}

bgfx::TextureHandle PostProcessing::get_color_geometry_render_texture() {
    return getTexture(m_geometryPass, 0);
}

bgfx::TextureHandle PostProcessing::get_color_gui_render_texture() {
    return getTexture(m_guiPass, 0);
}
void PostProcessing::on_awake() {

    m_shader.load_shader("bump", "vs_bump.bin", "fs_bump.bin");
    m_uniformGeometry = bgfx::createUniform("m_albedo", bgfx::UniformType::Sampler);
    m_uniformGui = bgfx::createUniform("m_normal", bgfx::UniformType::Sampler);
}
void PostProcessing::on_destroy() {}

void PostProcessing::set_uniforms() {
    if (isValid(get_color_geometry_render_texture())) {
        log::info("is valid geo");
        bgfx::setTexture(0, m_uniformGeometry, get_color_geometry_render_texture());
    }
//    if (isValid(get_color_gui_render_texture())) {
//        log::info("is valid gui");
//        bgfx::setTexture(1, m_uniformGui, get_color_gui_render_texture());
//    }
}

}  // namespace components
}  // namespace knot