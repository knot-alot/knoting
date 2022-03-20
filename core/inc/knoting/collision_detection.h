#pragma once

#include <knoting/event_callback.h>

namespace knot {
namespace components {
class Collision_Detction {
   public:
    Collision_Detction();
    ~Collision_Detction();

    void on_awake();
    void on_destroy();

    void search_by_actor(PxActor* actor);
    void search_by_name(std::string name);
    /*
    PxActor* get_search_actor() { return m_event_callback->get()->get_search_actor(); }
    std::vector<contact_data> get_contact_data() { return m_event_callback->get()->get_contact_data(); }

    void print() { log::debug(m_event_callback->get()->get_contact_data().at(0).m_contact_point.x); }
    */

    PxActor* get_search_actor() { return m_event_callback->get_search_actor(); }
    std::vector<contact_data> get_contact_data() { return m_event_callback->get_contact_data(); }

   private:
    // std::shared_ptr<event_callback_ptr_wrapper> m_event_callback;
    Event_Callback* m_event_callback;
};
}  // namespace components
}  // namespace knot
