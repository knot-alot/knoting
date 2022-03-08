#include "knoting/framebuffer_manager.h"
#include <knoting/engine.h>
#include <knoting/log.h>

namespace knot {

FramebufferManager::FramebufferManager(Engine& engine) : m_engine(engine) {}

void FramebufferManager::on_awake() {
    uint64_t colorState = 0 | BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z |
                          BGFX_STATE_DEPTH_TEST_LESS;

    uint64_t depthState = 0 | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

    auto windowSize = m_engine.get_window_module().lock()->get_window_size();

    m_renderTextures[(size_t)FrameBufferType::Back] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::Depth] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::Color] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::PostProcess] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::Gui] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::ShadowOne] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::ShadowTwo] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::ShadowThree] = RenderTexture();
    m_renderTextures[(size_t)FrameBufferType::ShadowFour] = RenderTexture();

    m_renderTextures[(size_t)FrameBufferType::Back].create_color_texture(windowSize, 0);
    m_renderTextures[(size_t)FrameBufferType::Depth].create_depth_texture(windowSize, 0);
    m_renderTextures[(size_t)FrameBufferType::Color].create_render_texture(windowSize, 0);
    m_renderTextures[(size_t)FrameBufferType::PostProcess].create_render_texture(windowSize, 0);
    m_renderTextures[(size_t)FrameBufferType::Gui].create_render_texture(windowSize, 0);
    m_renderTextures[(size_t)FrameBufferType::ShadowOne].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures[(size_t)FrameBufferType::ShadowTwo].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures[(size_t)FrameBufferType::ShadowThree].create_depth_texture(vec2i(512), 0, false);
    m_renderTextures[(size_t)FrameBufferType::ShadowFour].create_depth_texture(vec2i(512), 0, false);

    m_renderTextures[(size_t)FrameBufferType::Back].update_state(depthState);
    m_renderTextures[(size_t)FrameBufferType::Depth].update_state(depthState);
    m_renderTextures[(size_t)FrameBufferType::Color].update_state(colorState);
    m_renderTextures[(size_t)FrameBufferType::PostProcess].update_state(colorState);
    m_renderTextures[(size_t)FrameBufferType::Gui].update_state(colorState);
    m_renderTextures[(size_t)FrameBufferType::ShadowOne].update_state(depthState);
    m_renderTextures[(size_t)FrameBufferType::ShadowTwo].update_state(depthState);
    m_renderTextures[(size_t)FrameBufferType::ShadowThree].update_state(depthState);
    m_renderTextures[(size_t)FrameBufferType::ShadowFour].update_state(depthState);

    recreate_framebuffer(windowSize.x, windowSize.y);
}

void FramebufferManager::on_update(double m_delta_time) {}
void FramebufferManager::on_late_update() {}

void FramebufferManager::on_destroy() {
    log::info("FramebufferManager destroyed");
}
FramebufferManager::~FramebufferManager() {}
void FramebufferManager::recreate_framebuffer(int width, int height) {
    log::warn("recreate fbos {}, {}", width, height);
    auto windowSize = m_engine.get_window_module().lock()->get_window_size();

    {
        auto backFB = m_renderTextures[(size_t)FrameBufferType::Back].get_framebuffer();
        bgfx::setViewName(backFB.idx, "back buffer");
        bgfx::setViewClear(backFB.idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(backFB.idx, 0, 0, bgfx::BackbufferRatio::Equal);
        bgfx::setViewFrameBuffer(backFB.idx, backFB);
    }
    {
        auto depthFB = m_renderTextures[(size_t)FrameBufferType::Depth].get_framebuffer();
        bgfx::setViewName(depthFB.idx, "depth pass");
        bgfx::setViewClear(depthFB.idx, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(depthFB.idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(depthFB.idx, depthFB);
    }
    {
        auto colorFB = m_renderTextures[(size_t)FrameBufferType::Color].get_framebuffer();
        bgfx::setViewName(colorFB.idx, "color pass");
        bgfx::setViewClear(colorFB.idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(colorFB.idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(colorFB.idx, colorFB);
    }
    {
        auto postProcessFB = m_renderTextures[(size_t)FrameBufferType::PostProcess].get_framebuffer();
        bgfx::setViewName(postProcessFB.idx, "post process pass");
        bgfx::setViewClear(postProcessFB.idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(postProcessFB.idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(postProcessFB.idx, postProcessFB);
    }
    {
        auto guiFB = m_renderTextures[(size_t)FrameBufferType::Gui].get_framebuffer();
        bgfx::setViewName(guiFB.idx, "gui pass");
        bgfx::setViewClear(guiFB.idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(guiFB.idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(guiFB.idx, guiFB);
    }
    {
        auto shadowOneFB = m_renderTextures[(size_t)FrameBufferType::ShadowOne].get_framebuffer();
        bgfx::setViewName(shadowOneFB.idx, "shadow one pass");
        bgfx::setViewClear(shadowOneFB.idx, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(shadowOneFB.idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowOneFB.idx, shadowOneFB);
    }
    {
        auto shadowTwoFB = m_renderTextures[(size_t)FrameBufferType::ShadowTwo].get_framebuffer();
        bgfx::setViewName(shadowTwoFB.idx, "shadow two pass");
        bgfx::setViewClear(shadowTwoFB.idx, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(shadowTwoFB.idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowTwoFB.idx, shadowTwoFB);
    }
    {
        auto shadowThreeFB = m_renderTextures[(size_t)FrameBufferType::ShadowThree].get_framebuffer();
        bgfx::setViewName(shadowThreeFB.idx, "shadow three pass");
        bgfx::setViewClear(shadowThreeFB.idx, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(shadowThreeFB.idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowThreeFB.idx, shadowThreeFB);
    }
    {
        auto shadowFourFB = m_renderTextures[(size_t)FrameBufferType::ShadowFour].get_framebuffer();
        bgfx::setViewName(shadowFourFB.idx, "shadow four pass");
        bgfx::setViewClear(shadowFourFB.idx, BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(shadowFourFB.idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowFourFB.idx, shadowFourFB);
    }

    for (auto& rt : m_renderTextures) {
        if (rt.is_using_window_size()) {
            rt.recreate_render_texture(width, height);
        }
    }

    bgfx::ViewId order[] = {m_renderTextures[(size_t)FrameBufferType::Back].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::Depth].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::Color].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::PostProcess].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::Gui].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::ShadowOne].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::ShadowTwo].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::ShadowThree].get_framebuffer().idx,
                            m_renderTextures[(size_t)FrameBufferType::ShadowFour].get_framebuffer().idx};

    bgfx::setViewOrder(0, m_renderTextures.size(), order);

    //
    bgfx::setViewFrameBuffer((size_t)m_currentViewBuffer,
                             m_renderTextures[(size_t)m_currentViewBuffer].get_framebuffer());
}
// void FramebufferManager::set_current_view_buffer(FrameBufferType type) {
//     m_currentViewBuffer = type;
//     bgfx::setViewFrameBuffer((size_t)m_currentViewBuffer,
//                              m_renderTextures[(size_t)m_currentViewBuffer].get_framebuffer());
// }

void FramebufferManager::clear_all_framebuffers() {
    for (auto& rt : m_renderTextures) {
        rt.clear_framebuffer();
    }
}
bgfx::FrameBufferHandle FramebufferManager::get_framebuffer(FrameBufferType type) {
    return m_renderTextures[(size_t)type].get_framebuffer();
}

std::vector<bgfx::TextureHandle> FramebufferManager::get_texture_attachments(FrameBufferType type) {
    return m_renderTextures[(size_t)type].get_texture_attachments();
}

}  // namespace knot