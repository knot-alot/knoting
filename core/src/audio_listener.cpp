#include "knoting/audio_listener.h"
#include <knoting/scene.h>
namespace knot::components {

AudioListener::AudioListener(float range) {
    m_range = range;
}

FMOD_VECTOR AudioListener::get_position() {
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

quat AudioListener::get_rotation() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (goOpt) {
            return goOpt->get_component<components::Transform>().get_rotation();
        }
    }
}
void AudioListener::update_velocity() {
    // this needs to be changed to (pos-last_pos) / time_taken_since_last_frame_in_seconds to make doppler effect work
    FMOD_VECTOR vel;
    vel.x = (get_position().x - m_last_pos.x);
    vel.y = (get_position().y - m_last_pos.y);
    vel.z = (get_position().z - m_last_pos.z);
    m_last_pos = get_position();
    m_velocity = vel;
}

FMOD_VECTOR AudioListener::get_velocity() {
    return m_velocity;
}

}  // namespace knot::components