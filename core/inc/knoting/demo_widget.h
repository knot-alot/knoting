#pragma once
#include <bgfx/bgfx.h>
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/texture.h>
#include <knoting/widget.h>

namespace knot {

class DemoWidget : public Widget {
   public:
    DemoWidget(const std::string& name);
    ~DemoWidget();
    void on_widget_render() override;
    void setWinow(int x, int y) {
        m_x = x;
        m_y = y;
    };
    int m_x;
    int m_y;

    float m_health = 100;

    void setHealth(float currentHealth) { m_health = currentHealth; }
    void subtractHealth(float takeTaken) { m_health -= takeTaken; }

    std::shared_ptr<components::Texture> m_tex;
};

}  // namespace knot
