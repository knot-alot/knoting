#pragma once
#include <string>
namespace knot {

class Widget {
   public:
    Widget(std::string name) {}

    virtual void on_awake() {}
    virtual void on_widget_render() {}
    virtual void on_destroy() {}

   private:
    std::string m_name = "widget";
};
}  // namespace knot
