#include <knoting/event_callback.h>

namespace knot {
Event_Callback::Event_Callback() : m_search_actor(nullptr), m_is_search_by_actor(true) {}
Event_Callback::~Event_Callback() {}

void Event_Callback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            if (m_is_search_by_actor) {
                if (pairHeader.actors[0] == m_search_actor || pairHeader.actors[1] == m_search_actor) {
                    PxActor* otherActor =
                        (m_search_actor == pairHeader.actors[0]) ? pairHeader.actors[1] : pairHeader.actors[0];
                    PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(),
                                                 cp.patchCount, cp.contactCount);
                    vec3 point = vec3(iter.getContactPoint().x, iter.getContactPoint().y, iter.getContactPoint().z);
                    contact_data cd;
                    cd.m_contact_actor = otherActor;
                    cd.m_contact_point = point;
                    m_contact_data.push_back(cd);
                }
            } else {
                if (pairHeader.actors[0]->getName() == m_search_name ||
                    pairHeader.actors[1]->getName() == m_search_name) {
                    PxActor* otherActor = (m_search_name == pairHeader.actors[0]->getName()) ? pairHeader.actors[1]
                                                                                             : pairHeader.actors[0];
                    PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(),
                                                 cp.patchCount, cp.contactCount);
                    vec3 point = vec3(iter.getContactPoint().x, iter.getContactPoint().y, iter.getContactPoint().z);
                    contact_data cd;
                    cd.m_contact_actor = otherActor;
                    cd.m_contact_point = point;
                    m_contact_data.push_back(cd);
                }
            }
        }
    }
}

void Event_Callback::search_by_actor(PxActor* actor) {
    m_search_actor = actor;
    m_contact_data.clear();
}

void Event_Callback::search_by_name(std::string name) {
    m_search_name = name;
    m_contact_data.clear();
}

}  // namespace knot
