#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/widget.h>

namespace knot {

class Debug_gui : public Widget {
   public:
    Debug_gui(const std::string& name);
    ~Debug_gui();
    void on_widget_render() override;
    void setWinow(int x,int y){m_x = x;m_y =y;};
    void setFont(const char* path){
        ImGuiIO& io = ImGui::GetIO();
        font = io.Fonts-> AddFontFromFileTTF ( path , 15);
    };
    void setFrame(double frame){
        Frame = frame;
    }
   void setOpen(bool open){
      Open = open;
   }

   private:
    bool Open = false;
    double Frame ;
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
