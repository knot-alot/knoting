#pragma once
#include <bgfx/bgfx.h>
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/texture.h>
#include <knoting/widget.h>
namespace knot {

class WinLoseWidget : public Widget {
   public:
    WinLoseWidget(const std::string& name);
    ~WinLoseWidget();
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
    std::shared_ptr<components::Texture> m_backTex;
    bool isRedTeam;

    bool isGameOver = false;
    bool playerRedWins = false;
};

}  // namespace knot
