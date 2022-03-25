#pragma once

// clang-format off
#include <imgui.h>
#include <ImGuizmo.h>
#include <knoting/log.h>
#include <knoting/widget.h>
// clang-format on

namespace knot {

class DebugPhysics : public Widget {
   public:
    DebugPhysics(const std::string& name);
    ~DebugPhysics();
    void on_widget_render() override;

    void setWindow(int x, int y) {
        m_x = x;
        m_y = y;
    };

   private:
    ImDrawList* draw_list;
    int m_x;
    int m_y;
    bool active = false;
};

}  // namespace knot
