#include "knoting/Menu.h"
#include "bx/timer.h"
#include "knoting/engine.h"

knot::Menu::Menu(const std::string& name) : Widget(name) {}

void knot::Menu::on_widget_render() {
   // ImGui::ShowDemoWindow();
 //   Frame = 5;
   auto engineOpt = Engine::get_active_engine();
   if(!engineOpt){
       return;
   }
   auto &engine = engineOpt.value().get();
   if(!Start) {
       ImGui::SetNextWindowPos(ImVec2(0, 0)

       );
       ImGui::SetNextWindowSize(ImVec2(m_x, m_y)

       );
       ImGui::Begin("Menu", NULL,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                        ImGuiWindowFlags_NoMove);
       ImGui::SetNextWindowPos(ImVec2(m_x * 2 / 5, m_y / 8));
       ImGui::SetWindowFontScale(2.0f);
       ImGui::BeginChild("Text", ImVec2(m_x, m_y), true,
                         ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoMove);
       const char* text = "knoting game";
       ImGui::Text(text);
       ImGui::SetCursorPosY(m_y / 3.5);
       if (ImGui::Button("Start", ImVec2(150, 30))) {
           Start = true;
           engine.set_Pause(false);
       }

       ImGui::SetCursorPosY(m_y / 3.5 + 30);
       if (ImGui::Button("Quit", ImVec2(150, 30))) {
           engine.get_window_module().lock()->close();
       }
       ImGui::EndChild();

       ImGui::End();
   }
}

knot::Menu::~Menu() {
    log::info("DemoWidget destroyed");
}