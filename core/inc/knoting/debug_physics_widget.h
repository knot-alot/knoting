#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/widget.h>
#include <ImGuizmo.h>
namespace knot {

class DebugPhysics : public Widget {
   public:
    DebugPhysics(const std::string& name);
    ~DebugPhysics();
    void on_widget_render() override;
   private:
    ImDrawList* draw_list;

};

}  // namespace knot

