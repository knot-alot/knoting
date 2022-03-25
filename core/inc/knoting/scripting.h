#pragma once

#include <knoting/asset.h>
#include <knoting/game_object.h>
#include <knoting/scene.h>
#include <knoting/subsystem.h>
#include <knoting/quickjs.hpp>

namespace knot {

class Engine;

}

namespace knot {

class JSGameObject;
class JSStorage;

class Scripting : public Subsystem {
   public:
    Scripting(Engine& engine);

    void on_update(double deltaTime) override;
    void on_late_update() override;
    void on_awake() override;
    void on_destroy() override;

    std::shared_ptr<qjs::Runtime> get_script_runtime() const;
    std::shared_ptr<qjs::Context> get_script_context() const;

    void add_knoting_module();

    std::unordered_map<std::string, JSValue> storage;
    std::optional<std::reference_wrapper<qjs::Context::Module>> knoting_module;

   protected:
    std::optional<qjs::Value> m_ctor_transform;

    Engine& m_engine;
    std::shared_ptr<qjs::Runtime> m_runtime;
    std::shared_ptr<qjs::Context> m_context;
};

class JSObjectBase {
   public:
    void set_uuid(uuid id) { m_id = id; }

    std::optional<GameObject> retrieve_game_object() const {
        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt)
            return std::nullopt;

        auto& scene = sceneOpt.value().get();
        auto gameObjectOpt = scene.get_game_object_from_id(m_id);

        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt;
    }

   protected:
    uuid m_id;
};

class JSGameObject : public JSObjectBase {
   public:
    uuid get_id() const { return m_id; }

    int get_handle() const {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return -1;
        return static_cast<int>(gameObjectOpt.value().get_handle());
    }

    bool has_no_components() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return true;
        return gameObjectOpt.value().has_no_components();
    }

    bool has_component(const std::string& name);

    JSValue get_component(const std::string& name);

    JSValue add_component(const std::string& name);

    void remove_component(const std::string& name);
};

}  // namespace knot
