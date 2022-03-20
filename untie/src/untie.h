
#pragma once

#include <knoting/engine.h>

namespace knot {
class Window;
}

namespace knot {
class Untie {
   public:
    Untie();

    void run();

   private:
    std::unique_ptr<knot::Engine> m_engine;
};

}  // namespace knot
