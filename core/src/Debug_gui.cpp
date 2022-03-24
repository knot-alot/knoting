#include "knoting/Debug_gui.h"
#include "bx/timer.h"

knot::Debug_gui::Debug_gui(const std::string& name) : Widget(name) {}

void knot::Debug_gui::on_widget_render() {
    // ImGui::ShowDemoWindow();

    if (Open) {
        ImGui::SetNextWindowPos(

            ImVec2(0, 0), ImGuiCond_FirstUseEver

        );
        ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
        ImGui::Begin("Menu", NULL

        );

        ImGui::Text("FPS : ");
        ImGui::SameLine();
        ImGui::Text("% 7.3f[fps]", Frame);
        ImGui::Text("Update bgfx time:");
        ImGui::SameLine();
        ImGui::Text("% 7.3f[ms]", bgfx_time_cost);
        ImGui::Text("Update Physic time:");
        ImGui::SameLine();
        ImGui::Text("% 7.3f[microsecond]", Phy_time_cost);
        ImGui::Text("Update Gui time:");
        ImGui::SameLine();
        ImGui::Text("% 7.3f[microsecond]", GUI_time_cost);
        ImGui::Text("Physic detection:");
        ImGui::Text(PhysicName.c_str());

        ImGui::End();
        if (flag >= 20) {
            PhysicName.clear();
            flag = 0;
        }
        flag++;
    }
}

knot::Debug_gui::~Debug_gui() {
    log::info("DemoWidget destroyed");
}