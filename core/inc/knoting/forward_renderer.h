#pragma once

#include <knoting/subsystem.h>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <knoting/log.h>

#include <knoting/shader_program.h>
#include <knoting/mesh.h>
#include "../../../third/bgfx/bgfx/include/bgfx/bgfx.h" //TODO fix this for header files should be <bgfx/bgfx.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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

    ShaderProgram m_shaderProgram;
    Mesh m_cube;

    float m_timePassed = 0.01f;
    const uint32_t m_clearColor = 0x303030ff;
    Engine& m_engine;

};

}  // namespace knot
