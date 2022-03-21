#pragma once

#include <bgfx/bgfx.h>
#include <knoting/types.h>

namespace knot {

class RenderTexture {
   public:
    void create_render_texture(const vec2i& size, const uint64_t& flags, const bool useWindowSize = true);
    void create_depth_texture(const vec2i& size, const uint64_t& flags, const bool useWindowSize = true);
    void create_color_texture(const vec2i& size, const uint64_t& flags, const bool useWindowSize = true);

    void update_state(const uint64_t& state);
    void set_state();
    void set_view_projection(const mat4& view, const mat4& projection);

    void recreate_render_texture(int width, int height);
    void destroy();
    bool is_using_window_size() { return m_useWindowSize; };
    void clear_framebuffer();
    uint32_t get_clear_color() { return m_clearColor; };
    void set_clear_color(uint32_t color) { m_clearColor = color; };

    bgfx::FrameBufferHandle get_framebuffer() { return m_framebufferHandle; };
    std::vector<bgfx::TextureHandle> get_texture_attachments() { return m_renderTextureHandle; };

   private:
    vec2i m_size;
    bool m_useWindowSize = true;

    uint64_t m_state;
    uint64_t m_flags;
    uint16_t m_id;

    bgfx::FrameBufferHandle m_framebufferHandle;
    std::vector<bgfx::TextureHandle> m_renderTextureHandle;

    uint32_t m_clearColor = 0x00000000;
};

}  // namespace knot
