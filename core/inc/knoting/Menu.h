#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/widget.h>
namespace knot {

class Menu : public Widget {
   public:
    Menu(const std::string& name);
    ~Menu();
    void on_widget_render() override;
    void setWinow(int x, int y) {
        m_x = x;
        m_y = y;
    };
    void setFont(const char* path) {
        ImGuiIO& io = ImGui::GetIO();
        font = io.Fonts->AddFontFromFileTTF(path, 15);
    };
    bool get_quit_click() { return Quit_button_click; }
    bool get_Pause_click() { return Pause_button_click; }
   private:
    bool Pause_button_click = false;
    bool Quit_button_click = false;
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
