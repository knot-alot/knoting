#include "knoting/audio_source.h"
#include <knoting/engine.h>
#include <knoting/scene.h>

namespace knot::components {

AudioSource::AudioSource(const std::filesystem::path& path)
    : Asset{AssetType::Audio, ""}, m_sound(nullptr), m_path(path) {}

AudioSource::~AudioSource() = default;

void AudioSource::on_awake() {
    auto engOpt = Engine::get_active_engine();
    if (!engOpt) {
        return;
    }
    auto engineWrapper = engOpt.value();
    auto& engine = engineWrapper.get();
    engine.get_audio_module().lock()->add_sound(this);
}

FMOD_VECTOR* AudioSource::get_position() const {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            vec3 position = goOpt->get_component<components::Transform>().get_position();
            FMOD_VECTOR pos = {position.x, position.y, position.z};
            return &pos;
        }
    }
}
void AudioSource::on_destroy() {
    m_sound->release();
}
void AudioSource::generate_default_asset() {}

}  // namespace knot::components