#pragma once

#include <imgui.h>
#include <knoting/log.h>
#include <knoting/widget.h>
#include <knoting/asset_manager.h>

namespace knot {

class Menu : public Widget {
   public:
    Menu(const std::string& name);
    ~Menu();
    void on_widget_render() override;
    void setWinow(int x, int y) {
        m_x = x;
        m_y = y;
    };

   private:
    bool Start = false;
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
