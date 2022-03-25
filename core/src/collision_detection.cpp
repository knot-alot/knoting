#include <knoting/collision_detection.h>
#include <knoting/engine.h>

namespace knot {
namespace components {

Collision_Detection::Collision_Detection() : m_event_callback(nullptr) {}

Collision_Detection::~Collision_Detection() {}

void Collision_Detection::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_event_callback = engine.get_physics_module().lock()->get_event_callback();
    }
}

void Collision_Detection::add_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    m_event_callback->add_search_actor(actor);
}

void Collision_Detection::add_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    m_event_callback->add_search_actor(actor);
}

void Collision_Detection::add_search_name(std::string name) {
    m_event_callback->add_search_name(name);
}

void Collision_Detection::remove_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    m_event_callback->remove_search_actor(actor);
}

void Collision_Detection::remove_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    m_event_callback->remove_search_actor(actor);
}

void Collision_Detection::remove_search_name(std::string name) {
    m_event_callback->remove_search_name(name);
}

}  // namespace components
}  // namespace knot
