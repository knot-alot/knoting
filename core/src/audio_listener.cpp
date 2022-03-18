#include "knoting/audio_listener.h"
#include <knoting/scene.h>
#include <glm/vec3.hpp>
namespace knot::components {

FMOD_VECTOR* AudioListener::get_position() {
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

FMOD_VECTOR* AudioListener::get_forward() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (!goOpt) {
            return nullptr;
        }
        auto& transform = goOpt->get_component<components::Transform>();

        auto forwardUnit = vec3(transform.forward()) / length(transform.forward());

        FMOD_VECTOR forward = {forwardUnit.x, forwardUnit.y, forwardUnit.z};
        return &forward;
    }
}

FMOD_VECTOR* AudioListener::get_up() {
    auto sceneOpt = Scene::get_active_scene();
    if (sceneOpt) {
        Scene& scene = sceneOpt.value();
        entt::entity handle = entt::to_entity(scene.get_registry(), *this);
        auto goOpt = scene.get_game_object_from_handle(handle);
        if (!goOpt) {
            return nullptr;
        }
        auto& transform = goOpt->get_component<components::Transform>();
        auto upUnit = vec3(transform.up()) / length(transform.up());

        FMOD_VECTOR up = {upUnit.x, upUnit.y, upUnit.z};
        return &up;
    }
}

}  // namespace knot::components