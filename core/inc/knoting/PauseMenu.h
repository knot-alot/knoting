#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/widget.h>
namespace knot {

class PauseMenu : public Widget {
   public:
    PauseMenu(const std::string& name);
    ~PauseMenu();
    void on_widget_render() override;
    void setWinow(int x, int y) {
        m_x = x;
        m_y = y;
    };
    void setFont(const char* path) {
        ImGuiIO& io = ImGui::GetIO();
        font = io.Fonts->AddFontFromFileTTF(path, 15);
    };

   private:
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
