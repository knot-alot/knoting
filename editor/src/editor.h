
#pragma once

#include <knoting/engine.h>

namespace knot {
class Window;
}

namespace knot {
class Editor {
   public:
    Editor();

    void run();

   private:
    std::unique_ptr<knot::Engine> m_engine;
};

}  // namespace knot
