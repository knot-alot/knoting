#include "knoting/audio_listener.h"
#include <knoting/scene.h>
#include <glm/vec3.hpp>
namespace knot::components {

vec3 AudioListener::get_position() {
    auto sceneOpt = Scene::get_active_scene();

    if (!sceneOpt) {
        return {0, 0, 0};
    }
    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (goOpt) {
        vec3 position = goOpt->get_component<components::Transform>().get_position();
        return position;
    }
}

quat AudioListener::get_rotation() {
    auto sceneOpt = Scene::get_active_scene();

    if (!sceneOpt) {
        return {1, 0, 0, 0};
    }

    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (goOpt) {
        return goOpt->get_component<components::Transform>().get_rotation();
    }
}

vec3 AudioListener::get_forward() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return {0, 0, 0};
    }

    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (!goOpt) {
        return {0, 0, 0};
    }
    auto& transform = goOpt->get_component<components::Transform>();

    return transform.forward();
}

vec3 AudioListener::get_up() {
    auto sceneOpt = Scene::get_active_scene();

    if (!sceneOpt) {
        return {0, 0, 0};
    }

    Scene& scene = sceneOpt.value();
    entt::entity handle = entt::to_entity(scene.get_registry(), *this);
    auto goOpt = scene.get_game_object_from_handle(handle);
    if (!goOpt) {
        return {0, 0, 0};
    }
    auto& transform = goOpt->get_component<components::Transform>();

    return transform.up();
}

}  // namespace knot::components