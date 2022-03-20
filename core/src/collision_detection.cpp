#include <knoting/collision_detection.h>
#include <knoting/engine.h>

namespace knot {
namespace components {
Collision_Detction::Collision_Detction() : m_event_callback(nullptr) {}
Collision_Detction::~Collision_Detction() {}

void Collision_Detction::on_awake(){
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_event_callback = engine.get_physics_module().lock()->get_event_callback();
    }
}
void Collision_Detction::on_destroy() {}
/*
void Collision_Detction::search_by_actor(PxActor* actor) {
    m_event_callback->get()->search_by_actor(actor);
}

void Collision_Detction::search_by_name(std::string name) {
    m_event_callback->get()->search_by_name(name);
}
*/
void Collision_Detction::search_by_actor(PxActor* actor) {
    m_event_callback->search_by_actor(actor);
}

void Collision_Detction::search_by_name(std::string name) {
    m_event_callback->search_by_name(name);
}



}  // namespace components
}  // namespace knot
