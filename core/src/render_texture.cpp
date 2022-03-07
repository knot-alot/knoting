#include <knoting/log.h>
#include <knoting/render_texture.h>

namespace knot {

void RenderTexture::create_render_texture(const FrameBufferType& id, const vec2i& size, const uint64_t& flags) {
    m_flags = flags;

    bool validFlags = (m_flags & (BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LESS));
    if (!validFlags) {
        log::warn("invalid render texture flag using fallback");
        m_flags = BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LESS;
    }

    m_renderTextureHandle.emplace_back(
        bgfx::createTexture2D(size.x, size.y, false, 1, bgfx::TextureFormat::BGRA8, m_flags));
    m_renderTextureHandle.emplace_back(
        bgfx::createTexture2D(size.x, size.y, false, 1, bgfx::TextureFormat::D16, m_flags));
    m_framebufferHandle = bgfx::createFrameBuffer(m_renderTextureHandle.size(), m_renderTextureHandle.data());
}

void RenderTexture::create_depth_texture(const FrameBufferType& id, const vec2i& size, const uint64_t& flags) {
    m_flags = flags;

    bool validFlags = (m_flags & (BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL));
    if (!validFlags) {
        log::warn("invalid depth texture flag using fallback");
        m_flags = BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL;
    }

    m_renderTextureHandle.emplace_back(
        bgfx::createTexture2D(size.x, size.y, false, 1, bgfx::TextureFormat::D16, m_flags));
    m_framebufferHandle = bgfx::createFrameBuffer(m_renderTextureHandle.size(), m_renderTextureHandle.data());
}

void RenderTexture::recreate_render_texture(int width, int height) {
    m_size.x = width;
    m_size.y = height;

    bgfx::reset((uint32_t)width, (uint32_t)height, BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X2);
    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL, (uint32_t)m_clearColor);
    bgfx::setViewRect(m_id, 0, 0, width, height);
    bgfx::setViewFrameBuffer(m_id, BGFX_INVALID_HANDLE);  // DRAW INTO BACK BUFFER
    // TODO FB MANAGER SET ACTIVE FRAME BUFFER WITH "FrameBufferType"
    // bgfx::setViewFrameBuffer(m_id, m_framebufferHandle);
    // TODO SHADER TO PASS FBOS INTO // OR IMGUI IMAGE FOR EACH RT / FBO
}

void RenderTexture::set_view_projection(const mat4& view, const mat4& projection) {
    bgfx::setViewTransform(m_id, &view[0][0], &projection[0][0]);
}

void RenderTexture::set_state() {
    bgfx::setState(m_id, m_state);
}

void RenderTexture::update_state(const uint64_t& state) {
    m_state = state;
}

void RenderTexture::destroy() {
    for (auto& tex : m_renderTextureHandle) {
        bgfx::destroy(tex);
    }
    bgfx::destroy(m_framebufferHandle);
}

void RenderTexture::clear_framebuffer() {
    bgfx::setViewClear(m_id, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, (uint32_t)m_clearColor);
}

}  // namespace knot