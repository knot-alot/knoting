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
    return bgfx::getTexture(m_geometryPass, 2);
}

bgfx::TextureHandle PostProcessing::get_color_gui_render_texture() {
    return getTexture(m_guiPass, 2);
}

void PostProcessing::on_awake() {
    m_shader.load_shader("postprocess", "vs_pp.bin", "fs_pp.bin");
    m_uniformGeometry = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    m_uniformGui = bgfx::createUniform("m_normal", bgfx::UniformType::Sampler);
    m_albedo = AssetManager::load_asset<components::Texture>("UV_Grid_test.png").lock();
}
void PostProcessing::on_destroy() {}

void PostProcessing::set_uniforms() {
    if (isValid(m_geoTBH)) {
        log::info("is valid geo");
        bgfx::setTexture(0, m_uniformGeometry, m_albedo->get_texture_handle());
        //        bgfx::setTexture(1, m_uniformGeometry, m_geoTBH);
    }
}
void PostProcessing::set_color_render_texture(const bgfx::TextureHandle& tbh) {
    m_albedo->set_texture_handle(tbh);
    //    m_geoTBH = tbh;
}

}  // namespace components
}  // namespace knot