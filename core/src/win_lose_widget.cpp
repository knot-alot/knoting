#include <knoting/asset_manager.h>
#include <knoting/engine.h>
#include <knoting/texture.h>
#include <knoting/win_lose_widget.h>

knot::WinLoseWidget::WinLoseWidget(const std::string& name) : Widget(name) {
    m_backTex = AssetManager::load_asset<components::Texture>("you_win_lose.png").lock();

    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return;
    }
    auto& engine = engineOpt.value();
    auto cliModule = engine.get().get_client_module().lock();
    uint16_t playerNum = cliModule->get_client_num();

    isRedTeam = true;
    if (playerNum % 2 == 0) {
        isRedTeam = false;
    }

    if (isRedTeam) {
        m_tex = AssetManager::load_asset<components::Texture>("test_red.png").lock();
    } else {
        m_tex = AssetManager::load_asset<components::Texture>("test_blue.png").lock();
    }
}

void knot::WinLoseWidget::on_widget_render() {
    // ImGui::ShowDemoWindow();
    ImVec4 color;

    if (isGameOver) {
        std::string winText;

        if (playerRedWins) {
            color = ImVec4(1, 0.1, 0.1, 1);
            if (isRedTeam) {
                winText = "YOUR TEAM HAS WON";
            } else {
                winText = "YOUR TEAM HAS LOST";
            }
        } else {
            color = ImVec4(0.1, 0.1, 1.0, 1.0);
            if (isRedTeam) {
                winText = "YOUR TEAM HAS LOST";
            } else {
                winText = "YOUR TEAM HAS WON";
            }
        }

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize("winlosetext", ImVec2(m_x, m_y));

        ImGui::Begin("winlosetext", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

        ImTextureID imTexBack = (void*)m_backTex->get_texture_handle().idx;

        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                  // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                  // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // 50% opaque white
        ImGui::SetCursorPos(ImVec2(0, 0));
        ImGui::Image(imTexBack, ImVec2(m_x, m_y), uv_min, uv_max, tint_col);
        ImGui::SetCursorPos(ImVec2((m_x * 0.5) - 225, m_y * 0.5 - 125));
        ImGui::SetWindowFontScale(4.0);
        ImGui::TextColored(color, winText.c_str());
        //        ImGui::Text(winText.c_str());
        ImGui::End();
    }

    ImGui::Begin("DEBUG TEST WIN LOSE");
    ImGui::Checkbox("GAME OVER ", &isGameOver);
    ImGui::Checkbox("RED TEAM WIN ", &playerRedWins);
    ImGui::Checkbox("IS RED ", &isRedTeam);
    ImGui::End();
}

knot::WinLoseWidget::~WinLoseWidget() {
    log::info("WinLoseWidget destroyed");
}
