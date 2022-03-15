#include <knoting/aggregate.h>
#include <knoting/engine.h>

namespace knot {
namespace components {

Aggregate::Aggregate() : m_aggregates(nullptr), m_aggregate(nullptr), m_Physics(nullptr), m_Scene(nullptr) {}
Aggregate::~Aggregate() {}
void Aggregate::on_awake() {
    get_modules_from_physics();
}
void Aggregate::on_destory(){};

void Aggregate::add_aggregate(std::string name, PxU32 number_actors, bool self_collision) {
    for (int i = 0; i < m_aggregates->size(); i++) {
        if (m_aggregates->at(i)->get_name() == name) {
            log::error("already have same name aggregate");
            return;
        }
    }

    PxAggregate* aggregate = m_Physics->get()->createAggregate(number_actors, self_collision);
    std::shared_ptr<PxAggregate_ptr_wrapper> shared_aggregate =
        std::make_shared<PxAggregate_ptr_wrapper>(aggregate, name);
    m_aggregate = shared_aggregate;
    m_Scene->get()->addAggregate(*shared_aggregate->get_aggregate());
    m_aggregates->push_back(shared_aggregate);
}

void Aggregate::remove_aggregate(std::string name) {
    for (int i = 0; i < m_aggregates->size(); i++) {
        if (m_aggregates->at(i)->get_name() == name) {
            m_Scene->get()->removeAggregate(*m_aggregates->at(i)->get_aggregate());
            m_aggregates->erase(m_aggregates->begin() + i);
            return;
        }
    }
    log::error("do not have " + name + " aggregate");
}

void Aggregate::find_aggregate(std::string name) {
    for (int i = 0; i < m_aggregates->size(); i++) {
        if (m_aggregates->at(i)->get_name() == name) {
            m_aggregate = m_aggregates->at(i);
            return;
        }
    }
    log::error("can not find " + name + " aggregate");
    return;
}

void Aggregate::get_modules_from_physics() {
    auto engineOpt = Engine::get_active_engine();
    if (engineOpt) {
        Engine& engine = engineOpt.value();
        this->m_aggregates = engine.get_physics_module().lock()->get_Aggregates().lock();
        this->m_Physics = engine.get_physics_module().lock()->get_physics().lock();
        this->m_Scene = engine.get_physics_module().lock()->get_active_Scene().lock();
    }
}

}  // namespace components
}  // namespace knot
