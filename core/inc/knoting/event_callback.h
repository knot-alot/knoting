#pragma once

#include <PxPhysicsAPI.h>
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

    void search_by_actor(PxActor* actor);
    void search_by_name(std::string name);

    PxActor* get_search_actor() { return m_search_actor; }
    std::vector<contact_data> get_contact_data() { return m_contact_data; }

   private:
    PxActor* m_search_actor;
    std::string m_search_name;

    std::vector<contact_data> m_contact_data;
    bool m_is_search_by_actor;
};
}  // namespace knot
