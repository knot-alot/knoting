#pragma once

#include <array>
#include <memory>
#include <string>
#include <vector>

#include <knoting/subsystem.h>
#include <knoting/window.h>

namespace knot {
class Engine {
   public:
    Engine();
    ~Engine();

    void update_modules();

    std::weak_ptr<knot::Window> get_window_module() { return m_windowModule; }
    bool is_open();

   private:
    int m_windowWidth = 1024;
    int m_windowHeight = 768;
    std::string m_windowTitle = "hello knotting!";

   private:

    std::vector<std::shared_ptr<knot::Subsystem>> m_engineModules;
    std::shared_ptr<knot::Window> m_windowModule;
};

}  // namespace knot
