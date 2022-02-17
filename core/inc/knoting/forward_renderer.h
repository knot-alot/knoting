#pragma once

#include <bgfx/bgfx.h>
#include <knoting/light_data.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/shader_program.h>
#include <knoting/subsystem.h>
#include <knoting/texture.h>
#include <knoting/types.h>

namespace knot {

class Engine;

}
namespace knot {

class ForwardRenderer : public Subsystem {
   public:
    ForwardRenderer(Engine& engine);
    ~ForwardRenderer();

    void on_render();
    void on_post_render();

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

    void recreate_framebuffer(uint16_t width, uint16_t height, uint16_t id = 0);
    void clear_framebuffer(uint16_t id = 0);

   private:
    int get_window_width();
    int get_window_height();

    Engine& m_engine;
    LightData m_lightData;

   private:
    static constexpr uint32_t m_clearColor = 0x303030ff;
    float m_timePassed = 0.01f;
};

}  // namespace knot
