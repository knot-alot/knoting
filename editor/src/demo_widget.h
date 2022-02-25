#pragma once
#include "widget.h"
#include <imgui.h>
namespace knot {

class DemoWidget : public Widget {
   public:
    explicit DemoWidget(const std::string& name);
    void on_widget_render() override;
};

}  // namespace knot
