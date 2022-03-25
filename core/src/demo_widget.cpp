#include "knoting/demo_widget.h"
#include <knoting/asset_manager.h>
#include <knoting/engine.h>
#include <knoting/texture.h>

knot::DemoWidget::DemoWidget(const std::string& name) : Widget(name) {
    m_backTex = AssetManager::load_asset<components::Texture>("whiteTexture").lock();

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

void knot::DemoWidget::on_widget_render() {
    // ImGui::ShowDemoWindow();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetWindowSize("health", ImVec2(m_x, m_y));

    ImGui::Begin("health", NULL,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    static float i1 = 0;
    ImTextureID imTex = (void*)m_tex->get_texture_handle().idx;
    ImTextureID imTexBack = (void*)m_backTex->get_texture_handle().idx;

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                    // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                    // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);      // No tint
    ImVec4 tint_colBack = ImVec4(1.0f, 1.0f, 1.0f, 0.3f);  // No tint
    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // 50% opaque white
    ImGui::SetCursorPos(ImVec2(7.5, 15));
    ImGui::Image(imTex, ImVec2((m_health * m_x * 0.01f) - 15, 25), uv_min, uv_max, tint_col);
    ImGui::SetCursorPos(ImVec2(7.5, 15));
    ImGui::Image(imTexBack, ImVec2((m_x)-15, 25), uv_min, uv_max, tint_colBack, border_col);

    //    ImGui::SliderFloat("Health", &m_health, .1f, 100.0f);
    //  ImGui::Text("Num vertices:");
    ImGui::End();
}

knot::DemoWidget::~DemoWidget() {
    log::info("DemoWidget destroyed");
}
