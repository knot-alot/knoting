#include <knoting/aggregate.h>
#include <knoting/engine.h>

namespace knot {
namespace components {

Aggregate::Aggregate() : m_aggregates(nullptr), m_aggregate(nullptr), m_physics(nullptr), m_scene(nullptr) {}
Aggregate::~Aggregate() {}
void Aggregate::on_awake() {
    get_modules_from_physics();
}
void Aggregate::on_destory(){};

void Aggregate::add_aggregate(std::string name, PxU32 number_actors, bool self_collision) {
    auto is_even = [name](std::shared_ptr<PxAggregate_ptr_wrapper> i) { return i->get_name() == name; };
    auto result = std::find_if(m_aggregates->begin(), m_aggregates->end(), is_even);
    if (result != m_aggregates->end()) {
        log::error("already have same name aggregate");
    } else {
        PxAggregate* aggregate = m_physics->get()->createAggregate(number_actors, self_collision);
        m_aggregate = std::make_shared<PxAggregate_ptr_wrapper>(aggregate, name);
        m_scene->get()->addAggregate(*m_aggregate->get_aggregate());
        m_aggregates->push_back(m_aggregate);
    }
}

void Aggregate::remove_aggregate(std::string name) {
    auto is_even = [name](std::shared_ptr<PxAggregate_ptr_wrapper> i) { return i->get_name() == name; };
    auto result = std::find_if(m_aggregates->begin(), m_aggregates->end(), is_even);

    if (result != m_aggregates->end()) {
        m_scene->get()->removeAggregate(*result->get()->get_aggregate());
        m_aggregates->erase(result);
    } else {
        log::error("do not have " + name + " aggregate");
    }
}

void Aggregate::find_aggregate(std::string name) {
    auto is_even = [name](std::shared_ptr<PxAggregate_ptr_wrapper> i) { return i->get_name() == name; };
    auto result = std::find_if(m_aggregates->begin(), m_aggregates->end(), is_even);
    if (result != m_aggregates->end()) {
        m_aggregate = *result;
    } else {
        log::error("can not find " + name + " aggregate");
    }
}

void Aggregate::get_modules_from_physics() {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return;
    }
    Engine& engine = engineOpt.value();
    this->m_aggregates = engine.get_physics_module().lock()->get_aggregates().lock();
    this->m_physics = engine.get_physics_module().lock()->get_physics().lock();
    this->m_scene = engine.get_physics_module().lock()->get_active_Scene().lock();
}

}  // namespace components
}  // namespace knot
