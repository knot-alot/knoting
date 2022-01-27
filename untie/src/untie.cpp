
#include "untie.h"
#include <iostream>

namespace knot {

Untie::Untie() {
    m_engine = std::make_unique<knot::Engine>();
    m_window = m_engine->get_window_module();
}

void Untie::run() {
    std::cout << "here" << std::endl;
    while (m_window->is_open()) {
        m_engine->update_modules();
    }
}

}// namespace knot
