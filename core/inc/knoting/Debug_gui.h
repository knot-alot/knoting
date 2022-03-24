#pragma once
#include <imgui.h>
#include <knoting/log.h>
#include <knoting/physics.h>
#include <knoting/scene.h>
#include <knoting/widget.h>
#include "knoting/components.h"
#include "knoting/engine.h"

namespace knot {

class Debug_gui : public Widget {
   public:
    Debug_gui(const std::string& name);
    ~Debug_gui();
    void on_widget_render() override;
    void setWinow(int x, int y) {
        m_x = x;
        m_y = y;
    };
    void setFont(const char* path) {
        ImGuiIO& io = ImGui::GetIO();
        font = io.Fonts->AddFontFromFileTTF(path, 15);
    };
    void setFrame(double frame) { Frame = frame; }
    void setOpen(bool open) { Open = open; }
    void set_bgfx_Time(double time) { bgfx_time_cost = time; }
    void set_Phy_Time(double time) { Phy_time_cost = time; }
    void set_Gui_Time(double time) { GUI_time_cost = time; }
    std::string return_gameObj_name(PxActor* actor) {
        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt) {
            return "";
        }
        Scene& scene = sceneOpt.value();
        entt::registry& registry = scene.get_registry();

        auto entities = registry.view<components::RigidBody>();
        for (auto& e : entities) {
            auto goOpt = scene.get_game_object_from_handle(e);
            if (!goOpt) {
                continue;
            }
            GameObject go = goOpt.value();
            auto rigidbody = go.get_component<components::RigidBody>();
            bool same_actor = false;
            if (rigidbody.get_is_dynamic()) {
                rigidbody.get_dynamic().lock().get()->get() == actor ? same_actor = true : same_actor = false;
            } else {
                rigidbody.get_static().lock().get()->get() == actor ? same_actor = true : same_actor = false;
            }
            if (same_actor) {
                return go.get_component<components::Name>().get_name();
            }
        }
        return "no found";
    }
    void get_contact_data() {
        auto m_engine = Engine::get_active_engine();
        if (!m_engine) {
            return;
        }
        auto& engine = m_engine.value().get();
        std::vector<actor_contact_data> data =
            engine.get_physics_module().lock()->get_event_callback()->get_actor_contact_data();
        for (auto& e : data) {
            if (e.m_search_actor.m_is_dynamic) {
                PhysicName += return_gameObj_name((e.m_search_actor.m_search_dynamic.get()->get()));
            } else {
                PhysicName += return_gameObj_name((e.m_search_actor.m_search_static.get()->get()));
            }
            for (auto j : e.m_contact_data) {
                PhysicName += " Name :";
                PhysicName += return_gameObj_name(j.m_contact_actor);
                PhysicName += " Position :";
                PhysicName += " X :";
                PhysicName += std::to_string(j.m_contact_point.x);
                PhysicName += " Y :";
                PhysicName += std::to_string(j.m_contact_point.y);
                PhysicName += " Z :";
                PhysicName += std::to_string(j.m_contact_point.z);
                PhysicName += " Type :";
                if (j.type == contact_type::touch_found) {
                    PhysicName += "touch_found";
                } else {
                    PhysicName += "touch_persists";
                }
                PhysicName += "\n";
            }
            PhysicName += "\n";
        }
    }

   private:
    int flag = 0;
    std::string PhysicName;
    double bgfx_time_cost;
    double Phy_time_cost;
    double GUI_time_cost;
    bool Open = false;
    double Frame;
    ImFont* font;
    int m_x;
    int m_y;
};

}  // namespace knot
