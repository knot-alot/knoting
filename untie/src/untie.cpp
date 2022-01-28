#include "untie.h"
#include <iostream>

namespace knot {

Untie::Untie() {
    m_engine = std::make_unique<knot::Engine>();
}

void Untie::run() {
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
