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
   void set_bgfx_Time(double time){
       bgfx_time_cost = time;
   }
   void set_Phy_Time(double time){
       Phy_time_cost = time;
   }
   void set_Gui_Time(double time){
       GUI_time_cost = time;
   }
   private:
    double bgfx_time_cost;
    double Phy_time_cost;
    double GUI_time_cost;
    bool Open = false;
    double Frame ;
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
