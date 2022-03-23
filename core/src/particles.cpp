#include <knoting/particles.h>
#include <knoting/scene.h>
#include <stb_image.h>
#include <fstream>

namespace knot {
namespace components {
Particles::Particles() {}
Particles::~Particles() {}

void Particles::on_awake() {
    constexpr uint32_t default_max_particles = 1024;

    psInit(default_max_particles);
    ;
    EmitterSpriteHandle sprite = load_texture_2d(m_textureSlotPath);

    m_shape = EmitterShape::Sphere;
    m_direction_type = EmitterDirection::Outward;

    m_handle = psCreateEmitter(m_shape, m_direction_type, default_max_particles);
    m_uniforms.reset();

    m_uniforms.m_handle = sprite;

    m_uniforms.m_particlesPerSecond = 100;

    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        ;
        if (goOpt) {
            set_position(goOpt->get_component<components::Transform>().get_position());
        }
    }

    psUpdateEmitter(m_handle, &m_uniforms);
}
void Particles::on_destroy() {
    psDestroyEmitter(m_handle);
    psShutdown();
}

void Particles::set_position(vec3 position) {
    m_uniforms.m_position[0] = position.x;
    m_uniforms.m_position[1] = position.y;
    m_uniforms.m_position[2] = position.z;
}

void Particles::set_lookat(vec3 rotation) {
    m_uniforms.m_lookat[0] = rotation.x;
    m_uniforms.m_lookat[1] = rotation.y;
    m_uniforms.m_lookat[2] = rotation.z;
}

void Particles::set_max_particles(float maxParticles) {
    m_uniforms.m_maxParticles = maxParticles;
}

void Particles::set_gravity_scale(float gravity) {
    m_uniforms.m_gravityScale = gravity;
}

void Particles::set_shape(EmitterShape::Enum shape) {
    constexpr uint32_t default_max_particles = 1024;
    m_shape = shape;
    m_handle = psCreateEmitter(m_shape, m_direction_type, default_max_particles);
}

void Particles::set_direction_type(EmitterDirection::Enum directionType) {
    constexpr uint32_t default_max_particles = 1024;
    m_direction_type = directionType;
    m_handle = psCreateEmitter(m_shape, m_direction_type, default_max_particles);
}

void Particles::set_particles_per_second(uint32_t particlesPerSecond) {
    m_uniforms.m_particlesPerSecond = particlesPerSecond;
}

void Particles::set_min_life_span(float minLifeSpan) {
    m_uniforms.m_lifeSpan[0] = minLifeSpan;
}

void Particles::set_max_life_span(float maxLifeSpan) {
    m_uniforms.m_lifeSpan[1] = maxLifeSpan;
}

void Particles::set_ease_position(bx::Easing::Enum easeFun) {
    m_uniforms.m_easePos = easeFun;
}

void Particles::set_min_start_offset(float minOffset) {
    m_uniforms.m_offsetStart[0] = minOffset;
}

void Particles::set_max_start_offset(float maxOffset) {
    m_uniforms.m_offsetStart[1] = maxOffset;
}
void Particles::set_min_end_offset(float minOffset) {
    m_uniforms.m_offsetEnd[0] = minOffset;
}
void Particles::set_max_end_offset(float maxOffset) {
    m_uniforms.m_offsetEnd[1] = maxOffset;
}
void Particles::set_ease_particles_scale(bx::Easing::Enum easeFun) {
    m_uniforms.m_easeScale = easeFun;
}
void Particles::set_min_particles_start_scale(float minScale) {
    m_uniforms.m_scaleStart[0] = minScale;
}
void Particles::set_max_particles_start_scale(float maxScale) {
    m_uniforms.m_scaleStart[1] = maxScale;
}
void Particles::set_min_particles_end_scale(float minScale) {
    m_uniforms.m_scaleEnd[0] = minScale;
}
void Particles::set_max_particles_end_scale(float maxScale) {
    m_uniforms.m_scaleEnd[1] = maxScale;
}
void Particles::set_ease_blend(bx::Easing::Enum easeFun) {
    m_uniforms.m_easeBlend = easeFun;
}
void Particles::set_min_blend_start(float minBlend) {
    m_uniforms.m_blendStart[0] = minBlend;
}
void Particles::set_max_blend_start(float maxBlend) {
    m_uniforms.m_blendStart[1] = maxBlend;
}
void Particles::set_min_blend_end(float minBlend) {
    m_uniforms.m_blendEnd[0] = minBlend;
}
void Particles::set_max_blend_end(float maxBlend) {
    m_uniforms.m_blendEnd[1] = maxBlend;
}

void Particles::set_ease_color(bx::Easing::Enum easeFun) {
    m_uniforms.m_easeRgba = easeFun;
}
void Particles::set_rgba0(uint32_t rgba) {
    m_uniforms.m_rgba[0] = rgba;
}
void Particles::set_rgba1(uint32_t rgba) {
    m_uniforms.m_rgba[1] = rgba;
}
void Particles::set_rgba2(uint32_t rgba) {
    m_uniforms.m_rgba[2] = rgba;
}
void Particles::set_rgba4(uint32_t rgba) {
    m_uniforms.m_rgba[3] = rgba;
}

vec3 Particles::get_position() {
    return vec3(m_uniforms.m_position[0], m_uniforms.m_position[1], m_uniforms.m_position[2]);
}
vec3 Particles::get_lookat() {
    return vec3(m_uniforms.m_lookat[0], m_uniforms.m_lookat[1], m_uniforms.m_lookat[2]);
}
float Particles::get_max_particles() {
    return m_uniforms.m_maxParticles;
}
float Particles::get_gravity_scale() {
    return m_uniforms.m_gravityScale;
}
EmitterShape::Enum Particles::get_shape() {
    return m_shape;
}
EmitterDirection::Enum Particles::get_direction_type() {
    return m_direction_type;
}
uint32_t Particles::get_particles_per_second() {
    return m_uniforms.m_particlesPerSecond;
}
float Particles::get_min_life_span() {
    return m_uniforms.m_lifeSpan[0];
}
float Particles::get_max_life_span() {
    return m_uniforms.m_lifeSpan[1];
}
bx::Easing::Enum Particles::get_ease_position() {
    return m_uniforms.m_easePos;
}

float Particles::get_min_start_offset() {
    return m_uniforms.m_offsetStart[0];
}
float Particles::get_max_start_offset() {
    return m_uniforms.m_offsetStart[1];
}
float Particles::get_min_end_offset() {
    return m_uniforms.m_offsetEnd[0];
}
float Particles::get_max_end_offset() {
    return m_uniforms.m_offsetEnd[1];
}
bx::Easing::Enum Particles::get_ease_particles_scale() {
    return m_uniforms.m_easeScale;
}
float Particles::get_min_particles_start_scale() {
    return m_uniforms.m_scaleStart[0];
}
float Particles::get_max_particles_start_scale() {
    return m_uniforms.m_scaleStart[1];
}
float Particles::get_min_particles_end_scale() {
    return m_uniforms.m_scaleEnd[0];
}
float Particles::get_max_particles_end_scale() {
    return m_uniforms.m_scaleEnd[1];
}
bx::Easing::Enum Particles::get_ease_blend() {
    return m_uniforms.m_easeBlend;
}

float Particles::get_min_blend_start() {
    return m_uniforms.m_blendStart[0];
}
float Particles::get_max_blend_start() {
    return m_uniforms.m_blendStart[1];
}
float Particles::get_min_blend_end() {
    return m_uniforms.m_blendEnd[0];
}
float Particles::get_max_blend_end() {
    return m_uniforms.m_blendEnd[1];
}
bx::Easing::Enum Particles::get_ease_color() {
    return m_uniforms.m_easeRgba;
}
uint32_t Particles::get_rgba0() {
    return m_uniforms.m_rgba[0];
}
uint32_t Particles::get_rgba1() {
    return m_uniforms.m_rgba[1];
}
uint32_t Particles::get_rgba2() {
    return m_uniforms.m_rgba[2];
}
uint32_t Particles::get_rgba4() {
    return m_uniforms.m_rgba[3];
}
void Particles::update(float dt) {
    psUpdateEmitter(m_handle, &m_uniforms);
    psUpdate(dt);
}

void Particles::render(uint8_t view, const float* mtxView, const bx::Vec3& eye) {
    psRender(view, mtxView, eye);
}

EmitterSpriteHandle Particles::load_texture_2d(const std::string& path) {
    std::filesystem::path fsPath = AssetManager::get_resources_path().append(PATH_TEXTURE).append(path);

    if (!exists(fsPath)) {
        log::error("{} - does not Exist", fsPath.string());
    }

    // load image with stb_image
    glm::ivec2 imageSize;
    int channels;
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = stbi_load(fsPath.string().c_str(), &imageSize.x, &imageSize.y, &channels, 0);

    int numberOfLayers = 1;

    if (imageSize.x != imageSize.y) {
        log::error("image is not a square");
    }

    if (!data) {
        log::error("Failed to load image: {}", fsPath.string());
    }

    EmitterSpriteHandle handle = psCreateSprite(imageSize.x, imageSize.y, data, channels);

    stbi_set_flip_vertically_on_load(false);

    return handle;
}

float* Particles::mat4_to_float16(glm::mat4 m) {
    float a[16];
    const float* pSource = (const float*)glm::value_ptr(m);
    for (int i = 0; i < 16; i++) {
        a[i] = pSource[i];
    }
    return a;
}

}  // namespace components
}  // namespace knot
