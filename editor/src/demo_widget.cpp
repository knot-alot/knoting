#include "demo_widget.h"

knot::DemoWidget::DemoWidget(const std::string& name) : Widget(name){}

void knot::DemoWidget::on_widget_render()  {
    ImGui::ShowDemoWindow();
}
