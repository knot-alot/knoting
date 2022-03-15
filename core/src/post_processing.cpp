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
}  // namespace components
}  // namespace knot