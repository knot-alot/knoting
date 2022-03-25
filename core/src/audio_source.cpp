#include "knoting/audio_source.h"
#include <knoting/engine.h>
#include <knoting/scene.h>

namespace knot::components {

AudioSource::AudioSource(const std::string& path, bool loops)
    : Asset{AssetType::Audio, path}, m_sound(nullptr), m_path(path), m_loops(loops) {
    m_fullPath = AssetManager::get_resources_path().append("audio").append(path).string();
}

void AudioSource::on_awake() {
    auto engOpt = Engine::get_active_engine();
    if (!engOpt) {
        return;
    }
    auto engineWrapper = engOpt.value();

    auto& engine = engineWrapper.get();

    auto weakAudio = engine.get_audio_module();
    if (weakAudio.expired()) {
        return;
    }
    auto audio = weakAudio.lock();
    weakAudio.lock()->add_sound(*this);
}

vec3 AudioSource::get_position() const {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            vec3 position = goOpt->get_component<components::Transform>().get_position();
            return position;
        }
    }
}

void AudioSource::on_destroy() {
    m_sound->release();
}

}  // namespace knot::components