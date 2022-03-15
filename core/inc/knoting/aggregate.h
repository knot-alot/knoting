#pragma once

#include <knoting/physics.h>

namespace knot {
namespace components {
class Aggregate {
   public:
    Aggregate();
    ~Aggregate();
    void on_awake();
    void on_destory();
    std::weak_ptr<PxAggregate_ptr_wrapper> get_aggregate() { return m_aggregate; }
    void add_aggregate(std::string name, PxU32 number_actors, bool self_collision);
    void remove_aggregate(std::string name);
    void find_aggregate(std::string name);

    template <class Archive>
    void save(Archive& archive) const {}

    template <class Archive>
    void load(Archive& archive) {
        on_awake();
    }

   private:
    void get_modules_from_physics();

    std::shared_ptr<std::vector<std::shared_ptr<PxAggregate_ptr_wrapper>>> m_aggregates;
    std::shared_ptr<PxAggregate_ptr_wrapper> m_aggregate;
    std::shared_ptr<PxScene_ptr_wrapper> m_Scene;
    std::shared_ptr<PxPhysics_ptr_wrapper> m_Physics;
};
}  // namespace components
}  // namespace knot
