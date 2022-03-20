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

    void search_by_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void search_by_static(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void search_by_name(std::string name);

    std::vector<contact_data> get_contact_data() { return m_event_callback->get_contact_data(); }

   private:
    Event_Callback* m_event_callback;
};
}  // namespace components
}  // namespace knot
