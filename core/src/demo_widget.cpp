#include "knoting/demo_widget.h"
#include <knoting/asset_manager.h>
#include <knoting/texture.h>

knot::DemoWidget::DemoWidget(const std::string& name) : Widget(name) {
    m_tex = AssetManager::load_asset<components::Texture>("test_red.png").lock();
}

void knot::DemoWidget::on_widget_render() {
    // ImGui::ShowDemoWindow();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(m_x, m_y), ImGuiCond_FirstUseEver);

    ImGui::Begin("health", NULL,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    static float i1 = 0;
    ImTextureID imTex = (void*)m_tex->get_texture_handle().idx;

    ImVec2 uv_min = ImVec2(0.0f, 0.0f);                  // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f);                  // Lower-right
    ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // No tint
//    ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);  // 50% opaque white
    ImGui::Image(imTex, ImVec2((m_health * m_x * 0.01f) - 40, 25), uv_min, uv_max, tint_col);

    //    ImGui::SliderFloat("Health", &m_health, .1f, 100.0f);
    //  ImGui::Text("Num vertices:");
    ImGui::End();
}

knot::DemoWidget::~DemoWidget() {
    log::info("DemoWidget destroyed");
}
