#include "knoting/framebuffer_manager.h"
#include <knoting/log.h>

namespace knot {

FramebufferManager::FramebufferManager(Engine& engine) : m_engine(engine) {}

void FramebufferManager::on_awake() {
    uint64_t colorState = 0;
    colorState =
        BGFX_STATE_MSAA | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS;

    uint64_t depthState = 0;
    depthState = BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL;

    auto colorRT = std::make_shared<RenderTexture>();
    auto depthRT = std::make_shared<RenderTexture>();

    colorRT->create_render_texture(FrameBufferType::Color, vec2i(1024), 0);
    depthRT->create_depth_texture(FrameBufferType::Depth, vec2i(1024), 0);
    colorRT->update_state(colorState);
    depthRT->update_state(depthState);

    m_renderTextures.emplace_back(colorRT);
    m_renderTextures.emplace_back(depthRT);
}

void FramebufferManager::on_update(double m_delta_time) {}
void FramebufferManager::on_late_update() {}

void FramebufferManager::on_destroy() {
    for (auto& rt : m_renderTextures) {
        rt->destroy();
    }
    m_renderTextures.clear();
    log::info("FramebufferManager destroyed");
}
FramebufferManager::~FramebufferManager() {}
void FramebufferManager::recreate_framebuffer(int width, int height) {
    for (auto& rt : m_renderTextures) {
        if (rt->is_using_window_size()) {
            rt->recreate_render_texture(width, height);
        }
    }
}
void FramebufferManager::clear_all_framebuffers() {
    for (auto& rt : m_renderTextures) {
        rt->clear_framebuffer();
    }
}

}  // namespace knot