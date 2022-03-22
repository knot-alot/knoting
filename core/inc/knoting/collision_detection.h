#pragma once

#include <knoting/event_callback.h>

namespace knot {
namespace components {
class Collision_Detection {
   public:
    Collision_Detection();
    ~Collision_Detection();

    void on_awake();
    void on_destroy();

    void add_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void add_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void add_search_name(std::string name);
    void remove_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void remove_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void remove_search_name(std::string name);

    std::vector<contact_data> get_contact_data_by_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
        return m_event_callback->get_contact_data_by_actor(actor);
    }

    std::vector<contact_data> get_contact_data_by_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
        return m_event_callback->get_contact_data_by_actor(actor);
    }

    std::vector<contact_data> get_contact_data_by_name(std::string name) {
        return m_event_callback->get_contact_data_by_name(name);
    }
    std::vector<name_contact_data> get_name_contact_data() { return m_event_callback->get_name_contact_data(); }
    std::vector<actor_contact_data> get_actor_contact_data() { return m_event_callback->get_actor_contact_data(); }

   private:
    Event_Callback* m_event_callback;
};
}  // namespace components
}  // namespace knot
