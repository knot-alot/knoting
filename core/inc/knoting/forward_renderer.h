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

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

   private:
    // 1) SYSTEM : Shadow pass
    // 2) SYSTEM : Update Active Camera (Runtime / Editor)
    // 3) SYSTEM : Depth Pass (No Transparent Objects)
    // 4) SYSTEM : Color Render Pass
    // 5) SYSTEM : Skybox Render
    // 6) SYSTEM : Sorted Transparent Render Pass
    // 7) SYSTEM : Post Processing Stack

    void shadow_pass();
    void depth_pass();
    void color_pass();
    void skybox_pass();
    void transparent_pass();
    void post_process_pass();

    Engine& m_engine;
    LightData m_lightData;

   private:
    static constexpr uint32_t m_clearColor = 0x303030ff;
    float m_timePassed = 0.01f;
    float m_dt;
};

}  // namespace knot
