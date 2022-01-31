#include "untie.h"

#include <knoting/log.h>

#include <PxActor.h>
#include <iostream>

namespace knot {

Untie::Untie() {
    log::Logger::setup();
    m_engine = std::make_unique<knot::Engine>();
}

void Untie::run() {
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
