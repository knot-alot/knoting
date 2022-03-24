#include "knoting/Menu.h"
#include "bx/timer.h"
#include "knoting/engine.h"

knot::Menu::Menu(const std::string& name) : Widget(name) {}

void knot::Menu::on_widget_render() {
    // ImGui::ShowDemoWindow();
    //   Frame = 5;
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return;
    }
    auto& engine = engineOpt.value().get();
    if (!Start) {
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(m_x, m_y),ImGuiCond_FirstUseEver);

        ImGui::Begin("Menu", NULL ,ImGuiCond_FirstUseEver);

        ImGui::SetWindowFontScale(3.0f);


        const char* text = "knoting game";
        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5 + 50), 250));

        ImGui::Text(text);
        ImGui::SetWindowFontScale(2.0f);
        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5), 300));
        if (ImGui::Button("Start", ImVec2(150, 30))) {
            Start = true;
            engine.set_Pause(false);
        }

        ImGui::SetCursorPos(ImVec2(m_x / 2 - (150 * 0.5), 300 + 40));
        if (ImGui::Button("Quit", ImVec2(150, 30))) {
            engine.get_window_module().lock()->close();
        }

        ImGui::End();
    }
}

knot::Menu::~Menu() {
    log::info("DemoWidget destroyed");
}