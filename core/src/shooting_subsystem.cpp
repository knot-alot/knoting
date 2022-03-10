#include "knoting/shooting_subsystem.h"
#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/input_manager.h>
#include <knoting/scene.h>

namespace knot {
Shooting::Shooting(Engine& engine) : m_engine(engine), m_inManager(nullptr) {}

void Shooting::on_awake() {
    m_inManager = m_engine.get_window_module().lock()->get_input_manager();
}

void Shooting::on_update(double m_delta_time) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();

    auto players = registry.view<components::Transform, components::Health, components::Tag>();
    for (auto& player : players) {
        auto goOpt = scene.get_game_object_from_handle(player);
        if (!goOpt) {
            continue;
        }

        GameObject go = goOpt.value();
        auto& transform = go.get_component<components::Transform>();
        auto& health = go.get_component<components::Health>();

        // when shooting
        if (m_inManager->mouse_button_held_down(MouseButtonCode::Left)) {
            health.set_health(health.get_health() - 1);

            if (health.get_health() < 0) {
                health.set_health(0);
            }
        }
        // refill paint
        if (m_inManager->key_held_down(KeyCode::E)) {
            health.set_health(health.get_health() + 1);

            if (health.get_health() > health.get_max_health()) {
                health.set_health(health.get_max_health());
            }
        }
    }
}

void Shooting::on_destroy() {}

}  // namespace knot

// TODO if shooting key pressed: shoot bullets
