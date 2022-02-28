#pragma once
#include <imgui.h>
#include "widget.h"
namespace knot {

class DemoWidget : public Widget {
   public:
    explicit DemoWidget(const std::string& name);
    void on_widget_render() override;
};

}  // namespace knot
