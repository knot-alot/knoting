#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include "widget.h"
namespace knot {

class DemoWidget : public Widget {
   public:
    DemoWidget(const std::string& name);
    ~DemoWidget();
    void on_widget_render() override;
};

}  // namespace knot
