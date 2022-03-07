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

   private:
    std::vector<std::shared_ptr<RenderTexture>> m_renderTextures;
    Engine& m_engine;
};

}  // namespace knot
