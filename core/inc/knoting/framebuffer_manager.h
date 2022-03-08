#pragma once

#include <bgfx/bgfx.h>
#include <knoting/render_texture.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <memory>

namespace knot {

class Engine;

}

namespace knot {

// enum class MSAAFlags { NONE, MSAA_X2, MSAA_x4, MSAA_x8, MSAA_x16, LAST };

class FramebufferManager : public Subsystem {
   public:
    explicit FramebufferManager(Engine& engine);
    ~FramebufferManager();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void recreate_framebuffer(int width, int height);
    void clear_all_framebuffers();

    //    void set_current_view_buffer(FrameBufferType type);
    bgfx::FrameBufferHandle get_framebuffer(FrameBufferType type);
    std::vector<bgfx::TextureHandle> get_texture_attachments(FrameBufferType type);

   private:
    FrameBufferType m_currentViewBuffer = FrameBufferType::Color;
    std::array<RenderTexture, (size_t)FrameBufferType::LAST> m_renderTextures;
    Engine& m_engine;
};

}  // namespace knot
