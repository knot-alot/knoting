#include "knoting/PauseMenu.h"
#include "bx/timer.h"
#include "knoting/engine.h"

knot::PauseMenu::PauseMenu(const std::string& name) : Widget(name) {}

void knot::PauseMenu::on_widget_render() {
    // ImGui::ShowDemoWindow();
    //   Frame = 5;
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return;
    }
    auto& engine = engineOpt.value().get();

    if (engine.get_is_pause_menu()) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize("Pause", ImVec2(m_x, m_y));

        ImGui::Begin("Pause", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetWindowFontScale(3.0f);

        const char* text = "PAUSE";
        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5 - 25), 250));

        ImGui::Text(text);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5), 300));
        if (ImGui::Button("RESUME", ImVec2(150, 30))) {
            engine.switch_pause_menu();
            engine.set_Pause(false);
        }

        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5), 300 + 40));
        if (ImGui::Button("Quit", ImVec2(150, 30))) {
            engine.get_window_module().lock()->close();
        }

        ImGui::End();
    }
}

knot::PauseMenu::~PauseMenu() {
    log::info("DemoWidget destroyed");
}