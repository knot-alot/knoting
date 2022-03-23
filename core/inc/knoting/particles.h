#pragma once
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bimg/bimg.h>
#include <knoting/components.h>
#include <knoting/particle_system.h>

namespace knot {
namespace components {
class Particles {
   public:
    Particles();
    ~Particles();

    // For ECS
    void on_awake();
    void on_destroy();
    //=======

    void set_position(vec3 position);
    void set_lookat(vec3 rotation);
    void set_max_particles(float maxParticles);
    void set_gravity_scale(float gravity);
    void set_shape(EmitterShape::Enum shape);
    void set_direction_type(EmitterDirection::Enum directionType);
    void set_particles_per_second(uint32_t particlesPerSecond);
    void set_min_life_span(float minLifeSpan);
    void set_max_life_span(float maxLifeSpan);
    void set_ease_position(bx::Easing::Enum easeFun);
    void set_min_start_offset(float minOffset);
    void set_max_start_offset(float maxOffset);
    void set_min_end_offset(float minOffset);
    void set_max_end_offset(float maxOffset);
    void set_ease_particles_scale(bx::Easing::Enum easeFun);
    void set_min_particles_start_scale(float minScale);
    void set_max_particles_start_scale(float maxScale);
    void set_min_particles_end_scale(float minScale);
    void set_max_particles_end_scale(float maxScale);
    void set_ease_blend(bx::Easing::Enum easeFun);
    void set_min_blend_start(float minBlend);
    void set_max_blend_start(float maxBlend);
    void set_min_blend_end(float minBlend);
    void set_max_blend_end(float maxBlend);
    void set_ease_color(bx::Easing::Enum easeFun);
    void set_rgba0(uint32_t rgba);
    void set_rgba1(uint32_t rgba);
    void set_rgba2(uint32_t rgba);
    void set_rgba4(uint32_t rgba);
    void set_texture(const std::string& path);

    vec3 get_position();
    vec3 get_lookat();
    float get_max_particles();
    float get_gravity_scale();
    EmitterShape::Enum get_shape();
    EmitterDirection::Enum get_direction_type();
    uint32_t get_particles_per_second();
    float get_min_life_span();
    float get_max_life_span();
    bx::Easing::Enum get_ease_position();
    float get_min_start_offset();
    float get_max_start_offset();
    float get_min_end_offset();
    float get_max_end_offset();
    bx::Easing::Enum get_ease_particles_scale();
    float get_min_particles_start_scale();
    float get_max_particles_start_scale();
    float get_min_particles_end_scale();
    float get_max_particles_end_scale();
    bx::Easing::Enum get_ease_blend();
    float get_min_blend_start();
    float get_max_blend_start();
    float get_min_blend_end();
    float get_max_blend_end();
    bx::Easing::Enum get_ease_color();
    uint32_t get_rgba0();
    uint32_t get_rgba1();
    uint32_t get_rgba2();
    uint32_t get_rgba4();

    void update(float dt);
    void render(uint8_t view, const float* mtxView, const bx::Vec3& eye);

    EmitterSpriteHandle load_texture_2d(const std::string& path);

    float* mat4_to_float16(glm::mat4);

    template <class Archive>
    void save(Archive& archive) const {
        archive(CEREAL_NVP(m_uniforms), CEREAL_NVP(m_shape), CEREAL_NVP(m_direction_type),
                CEREAL_NVP(m_textureSlotPath));
    }

    template <class Archive>
    void load(Archive& archive) {
        archive(CEREAL_NVP(m_uniforms), CEREAL_NVP(m_shape), CEREAL_NVP(m_direction_type),
                CEREAL_NVP(m_textureSlotPath));
        on_awake();
    }

   private:
    EmitterUniforms m_uniforms;
    EmitterHandle m_handle;

    EmitterShape::Enum m_shape;
    EmitterDirection::Enum m_direction_type;
    std::string m_textureSlotPath = {"particles/glow1.png"};
};
}  // namespace components
}  // namespace knot
