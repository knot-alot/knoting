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
        auto backFB = m_renderTextures[(size_t)FrameBufferType::Back];
        bgfx::setViewName(backFB.get_framebuffer().idx, "back buffer");
        bgfx::setViewClear(backFB.get_framebuffer().idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, backFB.get_clear_color(),
                           1.0f, 0);
        bgfx::setViewRect(backFB.get_framebuffer().idx, 0, 0, bgfx::BackbufferRatio::Equal);
        bgfx::setViewFrameBuffer(backFB.get_framebuffer().idx, backFB.get_framebuffer());
    }
    {
        auto depthFB = m_renderTextures[(size_t)FrameBufferType::Depth];
        bgfx::setViewName(depthFB.get_framebuffer().idx, "depth pass");
        bgfx::setViewClear(depthFB.get_framebuffer().idx, BGFX_CLEAR_DEPTH, depthFB.get_clear_color(), 1.0f, 0);
        bgfx::setViewRect(depthFB.get_framebuffer().idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(depthFB.get_framebuffer().idx, depthFB.get_framebuffer());
    }
    {
        auto colorFB = m_renderTextures[(size_t)FrameBufferType::Color];
        bgfx::setViewName(colorFB.get_framebuffer().idx, "color pass");
        bgfx::setViewClear(colorFB.get_framebuffer().idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                           colorFB.get_clear_color(), 1.0f, 0);
        bgfx::setViewRect(colorFB.get_framebuffer().idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(colorFB.get_framebuffer().idx, colorFB.get_framebuffer());
    }
    {
        auto postProcessFB = m_renderTextures[(size_t)FrameBufferType::PostProcess];
        bgfx::setViewName(postProcessFB.get_framebuffer().idx, "post process pass");
        bgfx::setViewClear(postProcessFB.get_framebuffer().idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                           postProcessFB.get_clear_color(), 1.0f, 0);
        bgfx::setViewRect(postProcessFB.get_framebuffer().idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(postProcessFB.get_framebuffer().idx, postProcessFB.get_framebuffer());
    }
    {
        auto guiFB = m_renderTextures[(size_t)FrameBufferType::Gui];
        bgfx::setViewName(guiFB.get_framebuffer().idx, "gui pass");
        bgfx::setViewClear(guiFB.get_framebuffer().idx, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, guiFB.get_clear_color(),
                           1.0f, 0);
        bgfx::setViewRect(guiFB.get_framebuffer().idx, 0, 0, windowSize.x, windowSize.y);
        bgfx::setViewFrameBuffer(guiFB.get_framebuffer().idx, guiFB.get_framebuffer());
    }
    {
        auto shadowOneFB = m_renderTextures[(size_t)FrameBufferType::ShadowOne];
        bgfx::setViewName(shadowOneFB.get_framebuffer().idx, "shadow one pass");
        bgfx::setViewClear(shadowOneFB.get_framebuffer().idx, BGFX_CLEAR_DEPTH, shadowOneFB.get_clear_color(), 1.0f, 0);
        bgfx::setViewRect(shadowOneFB.get_framebuffer().idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowOneFB.get_framebuffer().idx, shadowOneFB.get_framebuffer());
    }
    {
        auto shadowTwoFB = m_renderTextures[(size_t)FrameBufferType::ShadowTwo];
        bgfx::setViewName(shadowTwoFB.get_framebuffer().idx, "shadow two pass");
        bgfx::setViewClear(shadowTwoFB.get_framebuffer().idx, BGFX_CLEAR_DEPTH, shadowTwoFB.get_clear_color(), 1.0f, 0);
        bgfx::setViewRect(shadowTwoFB.get_framebuffer().idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowTwoFB.get_framebuffer().idx, shadowTwoFB.get_framebuffer());
    }
    {
        auto shadowThreeFB = m_renderTextures[(size_t)FrameBufferType::ShadowThree];
        bgfx::setViewName(shadowThreeFB.get_framebuffer().idx, "shadow three pass");
        bgfx::setViewClear(shadowThreeFB.get_framebuffer().idx, BGFX_CLEAR_DEPTH, shadowThreeFB.get_clear_color(), 1.0f,
                           0);
        bgfx::setViewRect(shadowThreeFB.get_framebuffer().idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowThreeFB.get_framebuffer().idx, shadowThreeFB.get_framebuffer());
    }
    {
        auto shadowFourFB = m_renderTextures[(size_t)FrameBufferType::ShadowFour];
        bgfx::setViewName(shadowFourFB.get_framebuffer().idx, "shadow four pass");
        bgfx::setViewClear(shadowFourFB.get_framebuffer().idx, BGFX_CLEAR_DEPTH, shadowFourFB.get_clear_color(), 1.0f,
                           0);
        bgfx::setViewRect(shadowFourFB.get_framebuffer().idx, 0, 0, 512, 512);
        bgfx::setViewFrameBuffer(shadowFourFB.get_framebuffer().idx, shadowFourFB.get_framebuffer());
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
void FramebufferManager::set_active_framebuffer(FrameBufferType type) {
    m_currentViewBuffer = type;
}

}  // namespace knot