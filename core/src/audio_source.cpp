#include "knoting/audio_source.h"
#include <knoting/scene.h>

namespace knot::components {

AudioSource::AudioSource() : Asset{AssetType::Audio, m_path} {}

AudioSource::~AudioSource() = default;

void AudioSource::on_awake() {}

FMOD_VECTOR AudioSource::get_position() const {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            vec3 position = goOpt->get_component<components::Transform>().get_position();
            FMOD_VECTOR pos = {position.x, position.y, position.z};
            return pos;
        }
    }
}
}  // namespace knot::components