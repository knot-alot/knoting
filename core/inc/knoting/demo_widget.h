#pragma once
#include <bgfx/bgfx.h>
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/texture.h>
#include <knoting/widget.h>
#include <knoting/networked_client.h>
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

    std::shared_ptr<components::Texture> m_texRed;
    std::shared_ptr<components::Texture> m_texBlue;
    std::shared_ptr<components::Texture> m_backTex;
    std::shared_ptr<NetworkedClient> m_cliModule;
    std::shared_ptr<InputManager>m_input;
    bool isRedTeam;
    bool active = false;
};

}  // namespace knot
