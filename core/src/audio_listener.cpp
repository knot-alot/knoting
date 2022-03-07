#include "knoting/audio_listener.h"
#include <knoting/scene.h>
namespace knot::components {

AudioListener::AudioListener(float range) {
    m_range = range;
}

vec3 AudioListener::get_position() const {
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

quat AudioListener::get_rotation() const {
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

}  // namespace knot::components