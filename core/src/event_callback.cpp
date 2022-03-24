#include <knoting/event_callback.h>
#include <knoting/log.h>

namespace knot {

Event_Callback::Event_Callback() {}
Event_Callback::~Event_Callback() {}

void Event_Callback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
    for (PxU32 i = 0; i < nbPairs; i++) {
        const PxContactPair& cp = pairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
            if (check_is_actor_in_actor_contact_data(pairHeader.actors[0])) {
                push_data_for_actor(pairHeader, cp, true);

            } else if (check_is_actor_in_actor_contact_data(pairHeader.actors[1])) {
                push_data_for_actor(pairHeader, cp, false);
            }

            if (pairHeader.actors[0]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[0]->getName())) {
                    push_data_for_name(pairHeader, cp, true);
                }
            } else if (pairHeader.actors[1]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[1]->getName())) {
                    push_data_for_name(pairHeader, cp, false);
                }
            }
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
            if (check_is_actor_in_actor_contact_data(pairHeader.actors[0])) {
                change_contact_type_for_actor(pairHeader, cp, true);

            } else if (check_is_actor_in_actor_contact_data(pairHeader.actors[1])) {
                change_contact_type_for_actor(pairHeader, cp, false);
            }

            if (pairHeader.actors[0]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[0]->getName())) {
                    change_contact_type_for_name(pairHeader, cp, true);
                }
            } else if (pairHeader.actors[1]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[1]->getName())) {
                    change_contact_type_for_name(pairHeader, cp, false);
                }
            }
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST) {
            if (check_is_actor_in_actor_contact_data(pairHeader.actors[0])) {
                remove_data_for_actor(pairHeader, cp, true);

            } else if (check_is_actor_in_actor_contact_data(pairHeader.actors[1])) {
                remove_data_for_actor(pairHeader, cp, false);
            }

            if (pairHeader.actors[0]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[0]->getName())) {
                    remove_data_for_name(pairHeader, cp, true);
                }
            } else if (pairHeader.actors[1]->getName()) {
                if (check_is_name_in_name_contact_data(pairHeader.actors[1]->getName())) {
                    remove_data_for_name(pairHeader, cp, false);
                }
            }
        }
    }
}

void Event_Callback::add_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    if (check_is_actor_in_actor_contact_data(actor->get())) {
        log::error("add -- detection already have same actor");
        return;
    }
    search_actor sa;
    sa.m_is_dynamic = true;
    sa.m_search_dynamic = actor;
    sa.m_search_static = nullptr;
    actor_contact_data data;
    data.m_search_actor = sa;
    std::vector<contact_data> vcd;
    data.m_contact_data = vcd;
    m_actor_contact_data.push_back(data);
}

void Event_Callback::add_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    if (check_is_actor_in_actor_contact_data(actor->get())) {
        log::error("add -- detection already have same actor");
        return;
    }
    search_actor sa;
    sa.m_is_dynamic = false;
    sa.m_search_static = actor;
    sa.m_search_dynamic = nullptr;
    actor_contact_data data;
    data.m_search_actor = sa;
    std::vector<contact_data> vcd;
    data.m_contact_data = vcd;
    m_actor_contact_data.push_back(data);
}

void Event_Callback::add_search_name(std::string name) {
    if (check_is_name_in_name_contact_data(name)) {
        log::error("add -- detection already have same name");
        return;
    }
    name_contact_data data;
    data.m_name = name;
    std::vector<contact_data> vcd;
    data.m_contact_data = vcd;
    m_name_contact_data.push_back(data);
}

void Event_Callback::remove_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    if (!check_is_actor_in_actor_contact_data(actor->get())) {
        log::error("remove -- detection already have same actor");
        return;
    }
    auto i = get_iter_from_actor_contact_data(actor->get());
    m_actor_contact_data.erase(i);
}

void Event_Callback::remove_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    if (!check_is_actor_in_actor_contact_data(actor->get())) {
        log::error("remove -- detection do not have same actor");
        return;
    }
    auto i = get_iter_from_actor_contact_data(actor->get());
    m_actor_contact_data.erase(i);
}

void Event_Callback::remove_search_name(std::string name) {
    if (!check_is_name_in_name_contact_data(name)) {
        log::error("remove -- detection do not have same name");
        return;
    }
    auto i = get_iter_from_name_contact_data(name);
    m_name_contact_data.erase(i);
}

std::vector<contact_data> Event_Callback::get_contact_data_by_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
    if (check_is_actor_in_actor_contact_data(actor->get())) {
        auto i = get_iter_from_actor_contact_data(actor->get());
        return i->m_contact_data;
    }
    log::error("do not have search actor");
    std::vector<contact_data> cd;
    return cd;
}

std::vector<contact_data> Event_Callback::get_contact_data_by_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
    std::vector<actor_contact_data>::iterator iter;
    if (check_is_actor_in_actor_contact_data(actor->get())) {
        auto i = get_iter_from_actor_contact_data(actor->get());
        return i->m_contact_data;
    }
    log::error("do not have search actor");
    std::vector<contact_data> cd;
    return cd;
}

std::vector<contact_data> Event_Callback::get_contact_data_by_name(std::string name) {
    std::vector<name_contact_data>::iterator iter;
    if (check_is_name_in_name_contact_data(name)) {
        auto i = get_iter_from_name_contact_data(name);
        return i->m_contact_data;
    }
    log::error("do not have search name");
    std::vector<contact_data> cd;
    return cd;
}

bool Event_Callback::check_is_actor_in_actor_contact_data(PxActor* actor) {
    auto is_even = [actor](actor_contact_data i) {
        if (i.m_search_actor.m_is_dynamic) {
            return i.m_search_actor.m_search_dynamic->get() == actor;
        } else {
            return i.m_search_actor.m_search_static->get() == actor;
        }
    };
    auto result = std::find_if(m_actor_contact_data.begin(), m_actor_contact_data.end(), is_even);
    if (result != m_actor_contact_data.end()) {
        return true;
    }
    return false;
}

bool Event_Callback::check_is_actor_in_contact_data(PxActor* actor, std::vector<contact_data> vcd) {
    auto is_even = [actor](contact_data i) { return i.m_contact_actor == actor; };
    auto result = std::find_if(vcd.begin(), vcd.end(), is_even);
    if (result != vcd.end()) {
        return true;
    }
    return false;
}

std::vector<actor_contact_data>::iterator Event_Callback::get_iter_from_actor_contact_data(PxActor* actor) {
    auto is_even = [actor](actor_contact_data i) {
        if (i.m_search_actor.m_is_dynamic) {
            return i.m_search_actor.m_search_dynamic->get() == actor;
        } else {
            return i.m_search_actor.m_search_static->get() == actor;
        }
    };
    auto result = std::find_if(m_actor_contact_data.begin(), m_actor_contact_data.end(), is_even);
    return result;
}

std::vector<contact_data>::iterator Event_Callback::get_iter_from_contact_data(PxActor* actor,
                                                                               std::vector<contact_data> vcd) {
    auto is_even = [actor](contact_data i) { return i.m_contact_actor == actor; };
    auto result = std::find_if(vcd.begin(), vcd.end(), is_even);

    return result;
}

bool Event_Callback::check_is_name_in_name_contact_data(std::string name) {
    auto is_even = [name](name_contact_data i) { return i.m_name == name; };
    auto result = std::find_if(m_name_contact_data.begin(), m_name_contact_data.end(), is_even);
    if (result != m_name_contact_data.end()) {
        return true;
    }
    return false;
}

bool Event_Callback::check_is_name_in_name_contact_data(char* name) {
    std::string s_name(name);
    auto is_even = [s_name](name_contact_data i) { return i.m_name == s_name; };
    auto result = std::find_if(m_name_contact_data.begin(), m_name_contact_data.end(), is_even);
    if (result != m_name_contact_data.end()) {
        return true;
    }
    return false;
}

std::vector<name_contact_data>::iterator Event_Callback::get_iter_from_name_contact_data(std::string name) {
    auto is_even = [name](name_contact_data i) { return i.m_name == name; };
    auto result = std::find_if(m_name_contact_data.begin(), m_name_contact_data.end(), is_even);
    return result;
}

std::vector<name_contact_data>::iterator Event_Callback::get_iter_from_name_contact_data(char* name) {
    std::string s_name(name);
    auto is_even = [s_name](name_contact_data i) { return i.m_name == s_name; };
    auto result = std::find_if(m_name_contact_data.begin(), m_name_contact_data.end(), is_even);
    return result;
}

void Event_Callback::push_data_for_actor(const PxContactPairHeader& pairHeader,
                                         const PxContactPair& cp,
                                         bool is_first) {
    PxActor* otherActor;
    PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount,
                                 cp.contactCount);

    PxVec3 point = iter.getContactPoint();
    vec3 p = vec3(point.x, point.y, point.z);
    contact_data cd;
    cd.m_contact_point = p;

    if (is_first) {
        otherActor = pairHeader.actors[1];
        cd.m_contact_actor = otherActor;
        cd.type = contact_type::touch_found;
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[0]);
        i->m_contact_data.push_back(cd);

    } else {
        otherActor = pairHeader.actors[0];
        cd.m_contact_actor = otherActor;
        cd.type = contact_type::touch_found;
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[1]);
        i->m_contact_data.push_back(cd);
    }
}

void Event_Callback::change_contact_type_for_actor(const PxContactPairHeader& pairHeader,
                                                   const PxContactPair& cp,
                                                   bool is_first) {
    PxActor* otherActor;
    if (is_first) {
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[0]);
        otherActor = pairHeader.actors[1];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            j->type = contact_type::touch_persists;
        }

    } else {
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[1]);
        otherActor = pairHeader.actors[0];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            j->type = contact_type::touch_persists;
        }
    }
}

void Event_Callback::remove_data_for_actor(const PxContactPairHeader& pairHeader,
                                           const PxContactPair& cp,
                                           bool is_first) {
    PxActor* otherActor;
    if (is_first) {
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[0]);
        otherActor = pairHeader.actors[1];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            i->m_contact_data.erase(j);
        }

    } else {
        auto i = get_iter_from_actor_contact_data(pairHeader.actors[1]);
        otherActor = pairHeader.actors[0];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            i->m_contact_data.erase(j);
        }
    }
}

void Event_Callback::push_data_for_name(const PxContactPairHeader& pairHeader, const PxContactPair& cp, bool is_first) {
    PxActor* otherActor;
    PxContactStreamIterator iter(cp.contactPatches, cp.contactPoints, cp.getInternalFaceIndices(), cp.patchCount,
                                 cp.contactCount);

    PxVec3 point = iter.getContactPoint();
    vec3 p = vec3(point.x, point.y, point.z);
    contact_data cd;
    cd.m_contact_point = p;
    if (is_first) {
        otherActor = pairHeader.actors[1];
        cd.m_contact_actor = otherActor;
        cd.type = contact_type::touch_found;
        auto i = get_iter_from_name_contact_data(pairHeader.actors[0]->getName());
        i->m_contact_data.push_back(cd);

    } else {
        otherActor = pairHeader.actors[0];
        cd.m_contact_actor = otherActor;
        cd.type = contact_type::touch_found;
        auto i = get_iter_from_name_contact_data(pairHeader.actors[1]->getName());
        i->m_contact_data.push_back(cd);
    }
}

void Event_Callback::change_contact_type_for_name(const PxContactPairHeader& pairHeader,
                                                  const PxContactPair& cp,
                                                  bool is_first) {
    PxActor* otherActor;
    if (is_first) {
        auto i = get_iter_from_name_contact_data(pairHeader.actors[0]->getName());
        otherActor = pairHeader.actors[1];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            j->type = contact_type::touch_persists;
        }

    } else {
        auto i = get_iter_from_name_contact_data(pairHeader.actors[1]->getName());
        otherActor = pairHeader.actors[0];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            j->type = contact_type::touch_persists;
        }
    }
}

void Event_Callback::remove_data_for_name(const PxContactPairHeader& pairHeader,
                                          const PxContactPair& cp,
                                          bool is_first) {
    PxActor* otherActor;
    if (is_first) {
        auto i = get_iter_from_name_contact_data(pairHeader.actors[0]->getName());
        otherActor = pairHeader.actors[1];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            i->m_contact_data.erase(j);
        }

    } else {
        auto i = get_iter_from_name_contact_data(pairHeader.actors[1]->getName());
        otherActor = pairHeader.actors[0];
        auto is_even = [otherActor](contact_data i) { return i.m_contact_actor == otherActor; };
        auto j = std::find_if(i->m_contact_data.begin(), i->m_contact_data.end(), is_even);
        if (j != i->m_contact_data.end()) {
            i->m_contact_data.erase(j);
        }
    }
}

}  // namespace knot
