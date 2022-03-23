#include "knoting/demo_widget.h"

knot::DemoWidget::DemoWidget(const std::string& name) : Widget(name) {}

void knot::DemoWidget::on_widget_render() {
   //ImGui::ShowDemoWindow();

   ImGui::SetNextWindowPos(
       ImVec2(100.f, 10.0f)

   );
   ImGui::SetNextWindowSize(
       ImVec2( 170.0f,  200.0f)

   );
    ImGui::Begin("Stats"
                 , NULL
                 , ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoBackground|ImGuiWindowFlags_NoMove
    );

    static float i1 = 0;
    ImGui::SliderFloat("Health", &i1, -1.0f, 3.0f);
  //  ImGui::Text("Num vertices:");
    ImGui::End();
}

knot::DemoWidget::~DemoWidget() {
    log::info("DemoWidget destroyed");
}
