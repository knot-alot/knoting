#include <knoting/collision_detection.h>
#include <knoting/engine.h>

namespace knot {
namespace components {
Collision_Detction::Collision_Detction() : m_event_callback(nullptr) {}
Collision_Detction::~Collision_Detction() {}

void Collision_Detction::on_awake() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        m_event_callback = engine.get_physics_module().lock()->get_event_callback();
    }
}
void Collision_Detction::on_destroy() {}

void Collision_Detction::search_by_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    m_event_callback->search_by_dynamic(actor);
}

void Collision_Detction::search_by_static(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    m_event_callback->search_by_static(actor);
}

void Collision_Detction::search_by_name(std::string name) {
    m_event_callback->search_by_name(name);
}

}  // namespace components
}  // namespace knot
