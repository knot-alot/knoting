#include <knoting/event_callback.h>

namespace knot {

Event_Callback::Event_Callback()
    : m_search_dynamic(nullptr), m_search_static(nullptr), m_is_search_by_actor(true), m_is_dynamic(true) {}
Event_Callback::~Event_Callback() {}

void Event_Callback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            if (m_is_search_by_actor) {
                if (m_is_dynamic) {
                    if (pairHeader.actors[0] == m_search_dynamic->get() ||
                        pairHeader.actors[1] == m_search_dynamic->get()) {
                        push_data(pairHeader, cp);
                    }
                } else {
                    if (pairHeader.actors[0] == m_search_static->get() ||
                        pairHeader.actors[1] == m_search_static->get()) {
                        push_data(pairHeader, cp);
                    }
                }
            } else {
                if (pairHeader.actors[0]->getName() == m_search_name ||
                    pairHeader.actors[1]->getName() == m_search_name) {
                    push_data(pairHeader, cp);
                }
            }
        }
    }
}

void Event_Callback::search_by_dynamic(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    m_is_dynamic = true;
    m_is_search_by_actor = true;
    m_search_dynamic = actor;
    m_contact_data.clear();
}

void Event_Callback::search_by_static(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    m_is_dynamic = false;
    m_is_search_by_actor = true;
    m_search_static = actor;
    m_contact_data.clear();
}

void Event_Callback::search_by_name(std::string name) {
    m_is_search_by_actor = false;
    m_search_name = name;
    m_contact_data.clear();
}

void Event_Callback::push_data(const PxContactPairHeader& pairHeader, const PxContactPair& cp) {
    PxActor* otherActor;
    if (m_is_search_by_actor) {
        if (m_is_dynamic) {
            otherActor =
                (m_search_dynamic->get() == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
        } else {
            otherActor = (m_search_static->get() == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
        }
    } else {
        otherActor = (m_search_name == pairHeader.actors[0]->getName()) ? pairHeader.actors[1] : pairHeader.actors[0];
    }

    PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount,
                                 cp.contactCount);

    PxVec3 point = iter.getContactPoint();
    vec3 p = vec3(point.x, point.y, point.z);
    contact_data cd;
    cd.m_contact_actor = otherActor;
    cd.m_contact_point = p;
    m_contact_data.push_back(cd);
}

}  // namespace knot
