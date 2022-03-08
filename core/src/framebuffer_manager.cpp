#include "knoting/framebuffer_manager.h"
#include <knoting/engine.h>
#include <knoting/log.h>

namespace knot {

FramebufferManager::FramebufferManager(Engine& engine) : m_engine(engine) {}

void FramebufferManager::on_awake() {
    uint64_t colorState = 0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                          BGFX_STATE_DEPTH_TEST_LESS;

    uint64_t depthState = 0 | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

    m_renderTextures2[(size_t)FrameBufferType::Depth] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::Color] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::PostProcess] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::ShadowOne] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::ShadowTwo] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::ShadowThree] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::ShadowFour] = RenderTexture();
    m_renderTextures2[(size_t)FrameBufferType::Editor] = RenderTexture();

    m_renderTextures2[(size_t)FrameBufferType::Depth].create_depth_texture(vec2i(1024), 0);
    m_renderTextures2[(size_t)FrameBufferType::Color].create_render_texture(vec2i(1024), 0);
    m_renderTextures2[(size_t)FrameBufferType::PostProcess].create_render_texture(vec2i(1024), 0);
    m_renderTextures2[(size_t)FrameBufferType::ShadowOne].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures2[(size_t)FrameBufferType::ShadowTwo].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures2[(size_t)FrameBufferType::ShadowThree].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures2[(size_t)FrameBufferType::ShadowFour].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures2[(size_t)FrameBufferType::Editor].create_render_texture(vec2i(512), 0);

    m_renderTextures2[(size_t)FrameBufferType::Depth].update_state(depthState);
    m_renderTextures2[(size_t)FrameBufferType::Color].update_state(colorState);
    m_renderTextures2[(size_t)FrameBufferType::PostProcess].update_state(colorState);
    m_renderTextures2[(size_t)FrameBufferType::ShadowOne].update_state(depthState);
    m_renderTextures2[(size_t)FrameBufferType::ShadowTwo].update_state(depthState);
    m_renderTextures2[(size_t)FrameBufferType::ShadowThree].update_state(depthState);
    m_renderTextures2[(size_t)FrameBufferType::ShadowFour].update_state(depthState);
    m_renderTextures2[(size_t)FrameBufferType::Editor].update_state(colorState);
}

void FramebufferManager::on_update(double m_delta_time) {}
void FramebufferManager::on_late_update() {}

void FramebufferManager::on_destroy() {
    log::info("FramebufferManager destroyed");
}
FramebufferManager::~FramebufferManager() {}
void FramebufferManager::recreate_framebuffer(int width, int height) {
    for (auto& rt : m_renderTextures2) {
        if (rt.is_using_window_size()) {
            rt.recreate_render_texture(width, height);
        }
    }

    bgfx::ViewId order[] = {m_renderTextures2[(size_t)FrameBufferType::Depth].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::Color].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::PostProcess].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::ShadowOne].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::ShadowTwo].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::ShadowThree].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::ShadowFour].get_framebuffer().idx,
                            m_renderTextures2[(size_t)FrameBufferType::Editor].get_framebuffer().idx};

    bgfx::setViewOrder(0, m_renderTextures2.size(), order);

    bgfx::setViewFrameBuffer((size_t)m_currentViewBuffer,
                             m_renderTextures2[(size_t)m_currentViewBuffer].get_framebuffer());
}

void FramebufferManager::set_current_view_buffer(FrameBufferType type) {
    m_currentViewBuffer = type;
    auto window = m_engine.get_window_module().lock();
    vec2i size = window->get_window_size();
    window->recreate_framebuffer(size.x, size.y);
}

void FramebufferManager::clear_all_framebuffers() {
    for (auto& rt : m_renderTextures2) {
        rt.clear_framebuffer();
    }
}
bgfx::FrameBufferHandle FramebufferManager::get_framebuffer(FrameBufferType type) {
    return m_renderTextures2[(size_t)type].get_framebuffer();
}

std::vector<bgfx::TextureHandle> FramebufferManager::get_texture_attachments(FrameBufferType type) {
    return m_renderTextures2[(size_t)type].get_texture_attachments();
}

}  // namespace knot