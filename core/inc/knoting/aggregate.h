#pragma once

#include <knoting/physics.h>
#include <knoting/component.h>

namespace knot {
namespace components {

class Aggregate : public Component<Aggregate> {
   public:
    Aggregate();
    ~Aggregate();

    void on_awake();
    void on_destory();
    std::weak_ptr<PxAggregate_ptr_wrapper> get_aggregate() { return m_aggregate; }
    void add_aggregate(std::string name, PxU32 number_actors, bool self_collision);
    void remove_aggregate(std::string name);
    void find_aggregate(std::string name);

   private:
    void get_modules_from_physics();

    std::shared_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> m_aggregates;
    std::shared_ptr<PxAggregate_ptr_wrapper> m_aggregate;
    std::shared_ptr<PxScene_ptr_wrapper> m_scene;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_physics;
};
}  // namespace components
}  // namespace knot
