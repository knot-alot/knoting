#include "knoting/Debug_gui.h"
#include "bx/timer.h"

knot::Debug_gui::Debug_gui(const std::string& name) : Widget(name) {}

void knot::Debug_gui::on_widget_render() {

    // ImGui::ShowDemoWindow();

    if(Open){
        ImGui::SetNextWindowPos(

        ImVec2(0, 0)

    );
    ImGui::SetNextWindowSize(
        ImVec2( 150,  150)

    );
    ImGui::Begin("Menu"
                 , NULL

    );

    ImGui::Text("FPS : ");ImGui::SameLine();ImGui::Text("% 7.3f[fps]", Frame);


    ImGui::End();
}
}

knot::Debug_gui::~Debug_gui() {
    log::info("DemoWidget destroyed");
}