
#pragma once

#include "knoting/engine.h"

class Window;

namespace knot {
class Untie {
   public:
    Untie();

    void run();

   private:
    std::unique_ptr<knot::Engine> m_engine;
    std::shared_ptr<knot::Window> m_window;
};

}  // namespace knot
