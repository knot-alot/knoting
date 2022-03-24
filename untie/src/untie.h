
#pragma once

#include <knoting/Debug_gui.h>
#include <knoting/Menu.h>
#include <knoting/PauseMenu.h>
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
    bool open = true;
    std::shared_ptr<knot::PauseMenu> m_Pause_menu;
    std::shared_ptr<knot::Debug_gui> m_debug;
    std::shared_ptr<knot::Menu> m_menu;
    std::unique_ptr<knot::Engine> m_engine;
};

}  // namespace knot
