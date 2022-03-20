#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/types.h>
#include <string>
#include <vector>

using namespace physx;

namespace knot {

struct contact_data {
    vec3 m_contact_point;
    PxActor* m_contact_actor;
};

/*
class event_callback_ptr_wrapper {
   public:
    event_callback_ptr_wrapper(Event_Callback* event_callback = nullptr) : m_event_callback(event_callback) {}
    ~event_callback_ptr_wrapper() {}

    void set(Event_Callback* event_callback) { m_event_callback = event_callback; }
    Event_Callback* get() { return m_event_callback; }

   private:
    Event_Callback* m_event_callback;
};
*/

class Event_Callback : public PxSimulationEventCallback {
   public:
    Event_Callback();
    ~Event_Callback();

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Implements PxSimulationEventCallback
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);
    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
    virtual void onConstraintBreak(PxConstraintInfo*, PxU32) {}
    virtual void onWake(PxActor**, PxU32) {}
    virtual void onSleep(PxActor**, PxU32) {}
    virtual void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    void search_by_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void search_by_static(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void search_by_name(std::string name);

    std::vector<contact_data> get_contact_data() { return m_contact_data; }
    void clear_data() { m_contact_data.clear(); }

   private:
    void push_data(const PxContactPairHeader& pairHeader, const PxContactPair& cp);

    std::shared_ptr<PxDynamic_ptr_wrapper> m_search_dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> m_search_static;

    std::string m_search_name;

    std::vector<contact_data> m_contact_data;
    bool m_is_search_by_actor;
    bool m_is_dynamic;
};
}  // namespace knot
