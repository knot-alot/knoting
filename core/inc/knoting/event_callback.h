#pragma once

#include <PxPhysicsAPI.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/types.h>
#include <map>
#include <string>
#include <vector>

using namespace physx;

namespace knot {

struct contact_data {
    vec3 m_contact_point;
    PxActor* m_contact_actor;
    PxU32 type;
};

struct contact_type {
    enum Enum {
        touch_found = (1 << 0),
        touch_persists = (1 << 1),
    };
};

struct search_actor {
    std::shared_ptr<PxDynamic_ptr_wrapper> m_search_dynamic;
    std::shared_ptr<PxStatic_ptr_wrapper> m_search_static;
    bool m_is_dynamic;
};

struct actor_contact_data {
    search_actor m_search_actor;
    std::vector<contact_data> m_contact_data;
};

struct name_contact_data {
    std::string m_name;
    std::vector<contact_data> m_contact_data;
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

    void add_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void add_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void add_search_name(std::string name);

    void remove_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    void remove_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    void remove_search_name(std::string name);

    std::vector<contact_data> get_contact_data_by_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor);
    std::vector<contact_data> get_contact_data_by_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor);
    std::vector<contact_data> get_contact_data_by_name(std::string name);
    std::vector<actor_contact_data> get_actor_contact_data() { return m_actor_contact_data; }
    std::vector<name_contact_data> get_name_contact_data() { return m_name_contact_data; }

   private:
    void push_data_for_name(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);
    void change_contact_type_for_name(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);
    void remove_data_for_name(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);

    void push_data_for_actor(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);
    void change_contact_type_for_actor(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);
    void remove_data_for_actor(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first);

    bool check_is_name_in_name_contact_data(std::string name);
    bool check_is_name_in_name_contact_data(char* name);
    bool check_is_actor_in_actor_contact_data(PxActor* actor);
    bool check_is_actor_in_contact_data(PxActor* actor, std::vector<contact_data> vcd);
    std::vector<actor_contact_data>::iterator get_iter_from_actor_contact_data(PxActor* actor);
    std::vector<name_contact_data>::iterator get_iter_from_name_contact_data(std::string name);
    std::vector<name_contact_data>::iterator get_iter_from_name_contact_data(char* name);
    std::vector<contact_data>::iterator get_iter_from_contact_data(PxActor* actor, std::vector<contact_data> vcd);

    std::vector<actor_contact_data> m_actor_contact_data;
    std::vector<name_contact_data> m_name_contact_data;
};
}  // namespace knot
