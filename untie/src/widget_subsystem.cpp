#include "widget_subsystem.h"

namespace knot {
void WidgetSubsystem::on_awake() {}
void WidgetSubsystem::on_update(double m_delta_time) {}
void WidgetSubsystem::on_late_update() {}
void WidgetSubsystem::on_destroy() {
    m_widgets.clear();
}
void WidgetSubsystem::add_widget(std::shared_ptr<Widget> widget) {
    m_widgets.emplace_back(widget);
}
WidgetSubsystem::WidgetSubsystem(Engine& engine) : m_engine(engine) {}
}  // namespace knot