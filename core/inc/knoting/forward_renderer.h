#pragma once

#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>

#include <bgfx/bgfx.h>
#include <knoting/mesh.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>

namespace knot {

class Engine;

}
namespace knot {

class ForwardRenderer : public Subsystem {
   public:
    ForwardRenderer(Engine& engine);
    ~ForwardRenderer();

    void on_render();
    void render_pbr();
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

    components::ShaderProgram m_shaderProgram;
    components::Mesh m_cube;
    components::Texture m_colorTexture;
    components::Texture m_normalTexture;

    Engine& m_engine;

   private:
    static constexpr uint32_t m_clearColor = 0x303030ff;
    float m_timePassed = 0.01f;

    // TODO move to pipeline class
    bgfx::UniformHandle s_texColor;
    bgfx::UniformHandle s_texNormal;
    bgfx::UniformHandle u_lightPosRadius;
    bgfx::UniformHandle u_lightRgbInnerR;
    bgfx::TextureHandle m_textureColor;
    bgfx::TextureHandle m_textureNormal;
    uint16_t m_numLights;
    // end TODO
};

}  // namespace knot
