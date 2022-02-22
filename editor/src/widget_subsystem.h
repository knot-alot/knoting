#pragma once

#include <memory>
#include <vector>
#include "knoting/log.h"
#include "knoting/subsystem.h"
#include "widget.h"

namespace knot {
class Engine;
}

namespace knot {

class WidgetSubsystem : public Subsystem {
   public:
    explicit WidgetSubsystem(Engine& engine);
    void add_widget(std::shared_ptr<Widget> widget);

    void on_awake() override;
    void on_update(double m_delta_time) override;
    void on_late_update() override;
    void on_destroy() override;

   private:
    std::vector<std::shared_ptr<Widget>> m_widgets;
    Engine& m_engine;
};

}  // namespace knot
