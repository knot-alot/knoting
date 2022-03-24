#include <knoting/assert.h>
#include <knoting/components.h>
#include <knoting/engine.h>
#include <knoting/game_object.h>
#include <knoting/instance_script.h>
#include <knoting/scene.h>
#include <knoting/scripting.h>
#include <fstream>

namespace knot {

Scripting::Scripting(Engine& engine) : m_engine(engine) {}

void Scripting::on_awake() {
    m_runtime = std::make_shared<qjs::Runtime>();
    m_context = std::make_shared<qjs::Context>(*m_runtime);

    add_knoting_module();
}

void Scripting::on_update(double deltaTime) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto scripts = registry.view<components::InstanceScript>();

    for (auto entity : scripts) {
        auto goOpt = scene.get_game_object_from_handle(entity);
        if (!goOpt)
            continue;

        GameObject gameObject = goOpt.value();
        components::InstanceScript& instanceScript = gameObject.get_component<components::InstanceScript>();
        instanceScript.call_function(components::DefaultScriptFunction::Update, deltaTime);
    }
}

void Scripting::on_late_update() {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;

    Scene& scene = sceneOpt.value();
    entt::registry& registry = scene.get_registry();
    auto scripts = registry.view<components::InstanceScript>();

    for (auto entity : scripts) {
        auto goOpt = scene.get_game_object_from_handle(entity);
        if (!goOpt)
            continue;

        GameObject gameObject = goOpt.value();
        components::InstanceScript& instanceScript = gameObject.get_component<components::InstanceScript>();
        instanceScript.call_function(components::DefaultScriptFunction::LateUpdate);
    }
}

void Scripting::on_destroy() {}

std::shared_ptr<qjs::Context> Scripting::get_script_context() const {
    return m_context;
}

std::shared_ptr<qjs::Runtime> Scripting::get_script_runtime() const {
    return m_runtime;
}

// Implementation

JSValue create_js_object_from_name(const std::string& name) {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt) {
        return JS_UNDEFINED;
    }
    auto& engine = engineOpt.value().get();

    auto weakScripting = engine.get_scripting_module();
    if (weakScripting.expired()) {
        return JS_UNDEFINED;
    }
    auto scripting = weakScripting.lock();

    if (!scripting->knoting_module)
        return JS_UNDEFINED;

    auto module = scripting->knoting_module->get().get_module();
    if (!module)
        return JS_UNDEFINED;

    std::optional<qjs::Value> ctor;
    for (int i = 0; i < JS_GetModuleExportEntriesCount(module); ++i) {
        JSAtom atomExportName = JS_GetModuleExportEntryName(scripting->get_script_context()->ctx, module, i);
        std::string exportName = JS_AtomToCString(scripting->get_script_context()->ctx, atomExportName);

        if (exportName == name) {
            ctor = scripting->get_script_context()->newValue(
                JS_GetModuleExportEntry(scripting->get_script_context()->ctx, module, i));
            JS_FreeAtom(scripting->get_script_context()->ctx, atomExportName);
            break;
        }
        JS_FreeAtom(scripting->get_script_context()->ctx, atomExportName);
    }

    if (!ctor || !JS_IsFunction(scripting->get_script_context()->ctx, ctor->v)) {
        JS_ThrowInternalError(scripting->get_script_context()->ctx, "Component %s not valid", name.c_str());
        return JS_UNDEFINED;
    }

    if (!JS_IsObject(ctor->v)) {
        JS_ThrowInternalError(scripting->get_script_context()->ctx, "Component %s not valid", name.c_str());
        return JS_UNDEFINED;
    }

    qjs::Value obj = scripting->get_script_context()->newValue(
        JS_CallConstructor(scripting->get_script_context()->ctx, ctor->v, 0, nullptr));
    if (JS_IsException(obj.v)) {
        JS_ThrowInternalError(scripting->get_script_context()->ctx, "Component %s failed to be constructed",
                              name.c_str());
        scripting->get_script_context()->dumpException();
        return JS_UNDEFINED;
    }

    return obj.release();
}

template <typename... Args>
void js_throw_exception(const std::string& message, Args&&... args) {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return;

    auto& engine = engineOpt.value().get();
    auto weakScripting = engine.get_scripting_module();
    if (weakScripting.expired())
        return;
    auto scripting = weakScripting.lock();

    log::error(message, std::forward<Args>(args)...);
    JS_ThrowInternalError(scripting->get_script_context()->ctx, message.c_str(), args...);
    return;
}

bool JSGameObject::has_component(const std::string& name) {
    auto gameObjectOpt = retrieve_game_object();
    if (!gameObjectOpt)
        return false;

    // WARNING: Please, if someone has a nice metaprogramming way to do this, let me know.

    if (name == "aggregate") {
        return gameObjectOpt.value().has_component<components::Aggregate>();
    } else if (name == "audioListener") {
        return gameObjectOpt.value().has_component<components::AudioListener>();
    } else if (name == "audioSource") {
        return gameObjectOpt.value().has_component<components::AudioSource>();
    } else if (name == "editorCamera" || name == "camera") {
        return gameObjectOpt.value().has_component<components::EditorCamera>();
    } else if (name == "clientPlayer") {
        return gameObjectOpt.value().has_component<components::ClientPlayer>();
    } else if (name == "instanceMesh" || name == "mesh") {
        return gameObjectOpt.value().has_component<components::InstanceMesh>();
    } else if (name == "instanceScript" || name == "script") {
        return gameObjectOpt.value().has_component<components::InstanceScript>();
    } else if (name == "material") {
        return gameObjectOpt.value().has_component<components::Material>();
    } else if (name == "particles") {
        return gameObjectOpt.value().has_component<components::Particles>();
    } else if (name == "raycast" || name == "rayCast") {
        return gameObjectOpt.value().has_component<components::Raycast>();
    } else if (name == "rigidBody") {
        return gameObjectOpt.value().has_component<components::RigidBody>();
    } else if (name == "spotLight") {
        return gameObjectOpt.value().has_component<components::SpotLight>();
    } else if (name == "texture") {
        return gameObjectOpt.value().has_component<components::Texture>();
    } else if (name == "transform") {
        return gameObjectOpt.value().has_component<components::Transform>();
    } else if (name == "name") {
        return gameObjectOpt.value().has_component<components::Name>();
    } else if (name == "tag") {
        return gameObjectOpt.value().has_component<components::Tag>();
    } else if (name == "hierarchy") {
        return gameObjectOpt.value().has_component<components::Hierarchy>();
    }

    js_throw_exception("Component %s not valid", name.c_str());

    return false;
}

std::optional<std::shared_ptr<InputManager>> get_input_manager() {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return std::nullopt;

    auto& engine = engineOpt.value().get();
    auto weakWindow = engine.get_window_module();
    if (weakWindow.expired())
        return std::nullopt;

    auto window = weakWindow.lock();
    auto input = window->get_input_manager();

    return input;
}

bool input_key_pressed(int key) {
    if (key < 0 || key > static_cast<size_t>(KeyCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->key_pressed(static_cast<KeyCode>(key));
}

bool input_key_held_down(int key) {
    if (key < 0 || key > static_cast<size_t>(KeyCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->key_held_down(static_cast<KeyCode>(key));
}

bool input_key_on_trigger(int key) {
    if (key < 0 || key > static_cast<size_t>(KeyCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->key_on_trigger(static_cast<KeyCode>(key));
}

bool input_key_on_release(int key) {
    if (key < 0 || key > static_cast<size_t>(KeyCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->key_on_release(static_cast<KeyCode>(key));
}

bool input_pad_button_pressed(int pad, int button) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return false;

    if (button < 0 || button > static_cast<size_t>(PadButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->pad_button_pressed(static_cast<PadCode>(pad), static_cast<PadButtonCode>(button));
}

bool input_pad_button_held_down(int pad, int button) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return false;

    if (button < 0 || button > static_cast<size_t>(PadButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->pad_button_held_down(static_cast<PadCode>(pad), static_cast<PadButtonCode>(button));
}

bool input_pad_button_triggered(int pad, int button) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return false;

    if (button < 0 || button > static_cast<size_t>(PadButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->pad_button_triggered(static_cast<PadCode>(pad), static_cast<PadButtonCode>(button));
}

bool input_pad_button_released(int pad, int button) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return false;

    if (button < 0 || button > static_cast<size_t>(PadButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->pad_button_released(static_cast<PadCode>(pad), static_cast<PadButtonCode>(button));
}

double input_get_pad_axis(int pad, int joy_stick) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return 0.0f;

    if (joy_stick < 0 || joy_stick > static_cast<size_t>(JoyStickCode::Last))
        return 0.0f;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return 0.0f;

    return inputOpt->get()->get_pad_axis(static_cast<PadCode>(pad), static_cast<JoyStickCode>(joy_stick));
}

bool input_pad_present(int pad) {
    if (pad < 0 || pad > static_cast<size_t>(PadCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->pad_present(static_cast<PadCode>(pad));
}

bool input_mouse_button_pressed(int button) {
    if (button < 0 || button > static_cast<size_t>(MouseButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->mouse_button_pressed(static_cast<MouseButtonCode>(button));
}

bool input_mouse_button_held_down(int button) {
    if (button < 0 || button > static_cast<size_t>(MouseButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->mouse_button_held_down(static_cast<MouseButtonCode>(button));
}

bool input_mouse_button_triggered(int button) {
    if (button < 0 || button > static_cast<size_t>(MouseButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->mouse_button_triggered(static_cast<MouseButtonCode>(button));
}

bool input_mouse_button_released(int button) {
    if (button < 0 || button > static_cast<size_t>(MouseButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->mouse_button_released(static_cast<MouseButtonCode>(button));
}

bool input_mouse_double_clicked(int button) {
    if (button < 0 || button > static_cast<size_t>(MouseButtonCode::Last))
        return false;

    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->mouse_double_clicked(static_cast<MouseButtonCode>(button));
}

bool input_wheel_moved() {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return false;

    return inputOpt->get()->wheel_moved();
}

vec2 input_get_relative_position() {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return vec2(0.0f, 0.0f);

    return inputOpt->get()->get_relative_position();
}

vec2 input_get_absolute_position() {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return vec2(0.0f, 0.0f);

    return inputOpt->get()->get_absolute_position();
}

vec2 input_get_wheel_movement() {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return vec2(0.0f, 0.0f);

    return inputOpt->get()->get_wheel_movement();
}

void input_set_mouse_sensitivity(double sensitivity) {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return;

    inputOpt->get()->set_mouse_sensitivity(sensitivity);
}

void input_set_double_click_limit(double ms) {
    auto inputOpt = get_input_manager();
    if (!inputOpt)
        return;

    inputOpt->get()->set_double_click_limit(ms);
}

class JSTransform : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Transform>> get_transform() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Transform>();
    }

    void set_position(vec3 position) {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return;

        auto& transform = transformOpt.value().get();
        transform.set_position(position);
    }

    void set_scale(vec3 scale) {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return;

        auto& transform = transformOpt.value().get();
        transform.set_scale(scale);
    }

    void set_rotation(quat rotation) {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return;

        auto& transform = transformOpt.value().get();
        transform.set_rotation(rotation);
    }

    void set_rotation_euler(vec3 euler) {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return;

        auto& transform = transformOpt.value().get();
        transform.set_rotation_euler(euler);
    }

    vec3 get_position() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.get_position();
    }

    vec3 get_scale() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.get_scale();
    }

    quat get_rotation() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return quat(0.0f, 0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.get_rotation();
    }

    vec3 get_rotation_euler() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.get_rotation_euler();
    }

    vec3 get_forward() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.forward();
    }

    vec3 get_up() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.up();
    }

    vec3 get_right() {
        auto transformOpt = get_transform();
        if (!transformOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& transform = transformOpt.value().get();
        return transform.right();
    }
};

class JSRigidBody : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::RigidBody>> get_rigidbody() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::RigidBody>();
    }

    std::optional<std::reference_wrapper<components::RigidController>> get_rigid_controller() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::RigidController>();
    }

    JSValue get_actor() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return JS_UNDEFINED;

        auto& rigidbody = rigidbodyOpt.value().get();

        uuid* uuidPtr;
        if (rigidbody.get_is_dynamic()) {
            uuidPtr = reinterpret_cast<uuid*>(rigidbody.get_dynamic().lock().get()->get()->userData);
        } else {
            uuidPtr = reinterpret_cast<uuid*>(rigidbody.get_static().lock().get()->get()->userData);
        }

        JSValue obj = create_js_object_from_name("GameObject");
        auto opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));

        (*opaque)->set_uuid(*uuidPtr);

        return obj;
    }

    void set_rotation(vec3 rotation) {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return;

        auto& rigidbody = rigidbodyOpt.value().get();
        rigidbody.set_rotation(rotation);
    }

    void add_force(vec3 force) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.add_force(force);
    }

    vec3 get_position() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& rigidbody = rigidbodyOpt.value().get();
        return rigidbody.get_position();
    }

    quat get_rotation() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& rigidbody = rigidbodyOpt.value().get();
        return rigidbody.get_rotation();
    }

    vec3 get_rotation_euler() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& rigidbody = rigidbodyOpt.value().get();
        return degrees(eulerAngles(rigidbody.get_rotation()));
    }

    void set_position(vec3 position) {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return;

        auto& rigidbody = rigidbodyOpt.value().get();
        rigidbody.set_position(position);
    }

    std::string get_name() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return "";

        auto& rigidbody = rigidbodyOpt.value().get();
        return rigidbody.get_name();
    }

    void set_name(std::string name) {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return;

        auto& rigidbody = rigidbodyOpt.value().get();
        rigidbody.set_name(name);
    }

    float get_mass() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return 0.0f;

        auto& rigidController = rigidControllerOpt.value().get();
        return rigidController.get_mass();
    }

    bool is_dynamic() {
        auto rigidbodyOpt = get_rigidbody();
        if (!rigidbodyOpt)
            return false;

        auto& rigidbody = rigidbodyOpt.value().get();
        return rigidbody.get_is_dynamic();
    }

    bool is_kinematic() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return false;

        auto& rigidController = rigidControllerOpt.value().get();
        return !rigidController.get_is_kinematic();
    }

    float get_angular_damping() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return 0.0f;

        auto& rigidController = rigidControllerOpt.value().get();
        return rigidController.get_angular_damping();
    }

    float get_linear_damping() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return 0.0f;

        auto& rigidController = rigidControllerOpt.value().get();
        return rigidController.get_linear_damping();
    }

    vec3 get_angular_velocity() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& rigidController = rigidControllerOpt.value().get();
        return rigidController.get_angular_velocity();
    }

    vec3 get_linear_velocity() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return vec3(0.0f, 0.0f, 0.0f);

        auto& rigidController = rigidControllerOpt.value().get();
        return rigidController.get_linear_velocity();
    }

    void set_kinematic(bool toggle) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_kinematic(toggle);
    }

    void set_mass(float mass) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_mass(mass);
    }

    void set_angular_damping(float damping) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_angular_damping(damping);
    }

    void set_linear_damping(float damping) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_linear_damping(damping);
    }

    void set_angular_velocity(vec3 velocity) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_angular_velocity(velocity);
    }

    void set_linear_velocity(vec3 velocity) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.set_linear_velocity(velocity);
    }

    void add_torque(vec3 torque) {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.add_torque(torque);
    }

    void clear_force() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.clear_force();
    }

    void clear_torque() {
        auto rigidControllerOpt = get_rigid_controller();
        if (!rigidControllerOpt)
            return;

        auto& rigidController = rigidControllerOpt.value().get();
        rigidController.clear_torque();
    }
};

class JSHierarchy : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Hierarchy>> get_hierarchy() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Hierarchy>();
    }

    uuid get_parent() {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return uuid();

        auto& hierarchy = hierarchyOpt.value().get();
        auto parentOpt = hierarchy.get_parent();

        if (parentOpt)
            return parentOpt.value();
        else
            return uuid();
    }

    bool has_parent() {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return false;

        auto& hierarchy = hierarchyOpt.value().get();
        return hierarchy.has_parent();
    }

    bool has_children() {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return false;

        auto& hierarchy = hierarchyOpt.value().get();
        return hierarchy.has_children();
    }

    bool has_child(uuid child) {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return false;

        auto& hierarchy = hierarchyOpt.value().get();
        return hierarchy.has_child(child);
    }

    std::vector<uuid> get_children() {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return std::vector<uuid>();

        auto& hierarchy = hierarchyOpt.value().get();
        return hierarchy.get_children();
    }

    void add_child(uuid child) {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return;
        auto hierarchy = hierarchyOpt.value().get();

        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt)
            return;
        auto& scene = sceneOpt.value().get();

        auto goOpt = scene.get_game_object_from_id(child);
        if (!goOpt)
            return;

        auto go = goOpt.value();
        hierarchy.add_child(go);
    }

    void remove_child(uuid child) {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return;
        auto hierarchy = hierarchyOpt.value().get();

        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt)
            return;
        auto& scene = sceneOpt.value().get();

        auto goOpt = scene.get_game_object_from_id(child);
        if (!goOpt)
            return;

        auto go = goOpt.value();
        hierarchy.remove_child(go);
    }

    void set_parent(uuid parent) {
        auto hierarchyOpt = get_hierarchy();
        if (!hierarchyOpt)
            return;
        auto& hierarchy = hierarchyOpt.value().get();

        auto sceneOpt = Scene::get_active_scene();
        if (!sceneOpt)
            return;
        auto& scene = sceneOpt.value().get();

        auto goOpt = scene.get_game_object_from_id(parent);
        if (!goOpt)
            return;

        auto go = goOpt.value();
        hierarchy.set_parent(go);
    }
};

class JSName : public JSObjectBase {
   public:
    std::string get_name() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return "";

        auto& name = gameObjectOpt.value().get_component<components::Name>();
        return name.name;
    }

    void set_name(std::string name) {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return;

        auto& nameComp = gameObjectOpt.value().get_component<components::Name>();
        nameComp.name = name;
    }
};

class JSTag : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Tag>> get_tag() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Tag>();
    }

    std::string get_tag_name() {
        auto tagOpt = get_tag();
        if (!tagOpt)
            return "";

        auto& tag = tagOpt.value().get();
        return tag.get_tag();
    }

    void set_tag_name(std::string tagName) {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return;
        auto gameObject = gameObjectOpt.value();

        if (gameObject.has_component<components::Tag>()) {
            gameObject.remove_component<components::Tag>();
        }

        auto registeredTags = components::Tag::get_registered_tags();
        bool found = false;
        for (auto& registeredTag : registeredTags) {
            if (registeredTag == tagName) {
                found = true;
                break;
            }
        }

        if (!found) {
            js_throw_exception("Tag not found %s", tagName.c_str());
        }

        gameObject.add_component<components::Tag>(tagName);
    }

    void registerTag(const std::string& name) { components::Tag::register_tag(name); }

    void unregisterTag(const std::string& name) { components::Tag::unregister_tag(name); }

    std::vector<std::string> get_registered_tags() { return components::Tag::get_registered_tags(); }
};

class JSAudioSource : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::AudioSource>> get_audio_source() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::AudioSource>();
    }

    bool is_playing() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return false;

        auto& audioSource = audioSourceOpt.value().get();
        return audioSource.m_isPlaying;
    }

    bool is_paused() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return false;

        auto& audioSource = audioSourceOpt.value().get();
        return audioSource.m_isPaused;
    }

    void set_loops(bool loops) {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return;

        auto& audioSource = audioSourceOpt.value().get();
        audioSource.m_loops = loops;
    }

    bool get_loops() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return false;

        auto& audioSource = audioSourceOpt.value().get();
        return audioSource.m_loops;
    }

    void toggle() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return;

        auto& audioSource = audioSourceOpt.value().get();

        auto engineOpt = Engine::get_active_engine();
        if (!engineOpt)
            return;
        auto& engine = engineOpt.value().get();

        auto audioSystemWeak = engine.get_audio_module();
        if (audioSystemWeak.expired())
            return;
        auto audioSystem = audioSystemWeak.lock();

        audioSystem->toggle(audioSource);
    }

    void stop() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return;

        auto& audioSource = audioSourceOpt.value().get();

        auto engineOpt = Engine::get_active_engine();
        if (!engineOpt)
            return;
        auto& engine = engineOpt.value().get();

        auto audioSystemWeak = engine.get_audio_module();
        if (audioSystemWeak.expired())
            return;
        auto audioSystem = audioSystemWeak.lock();

        audioSystem->stop(audioSource);
    }

    void play() {
        auto audioSourceOpt = get_audio_source();
        if (!audioSourceOpt)
            return;

        auto& audioSource = audioSourceOpt.value().get();

        auto engineOpt = Engine::get_active_engine();
        if (!engineOpt)
            return;
        auto& engine = engineOpt.value().get();

        auto audioSystemWeak = engine.get_audio_module();
        if (audioSystemWeak.expired())
            return;
        auto audioSystem = audioSystemWeak.lock();

        audioSystem->play(audioSource);
    }
};

class JSEditorCamera : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::EditorCamera>> get_editor_camera() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::EditorCamera>();
    }

    void set_look_target(const glm::vec3& target) {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return;

        auto& editorCamera = editorCameraOpt.value().get();
        editorCamera.set_look_target(target);
    }

    void set_fov(float fov) {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return;

        auto& editorCamera = editorCameraOpt.value().get();
        editorCamera.set_fov(fov);
    }

    void set_z_near(float zNear) {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return;

        auto& editorCamera = editorCameraOpt.value().get();
        editorCamera.set_z_near(zNear);
    }

    void set_z_far(float zFar) {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return;

        auto& editorCamera = editorCameraOpt.value().get();
        editorCamera.set_z_far(zFar);
    }

    void set_move_speed(float speed) {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return;

        auto& editorCamera = editorCameraOpt.value().get();
        editorCamera.set_move_speed(speed);
    }

    glm::vec3 get_up() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return glm::vec3(0.0f);

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_up();
    }

    glm::vec3 get_look_target() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return glm::vec3(0.0f);

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_look_target();
    }

    float get_fov() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return 0.0f;

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_fov();
    }

    float get_z_near() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return 0.0f;

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_z_near();
    }

    float get_z_far() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return 0.0f;

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_z_far();
    }

    float get_move_speed() {
        auto editorCameraOpt = get_editor_camera();
        if (!editorCameraOpt)
            return 0.0f;

        auto& editorCamera = editorCameraOpt.value().get();
        return editorCamera.get_move_speed();
    }
};

class JSRaycast : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Raycast>> get_raycast() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Raycast>();
    }

    vec3 get_origin() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return vec3(0.0f);

        auto& raycast = raycastOpt.value().get();
        return raycast.get_origin();
    }

    vec3 get_unit_dir() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return vec3(0.0f);

        auto& raycast = raycastOpt.value().get();
        return raycast.get_unit_dir();
    }

    float get_max_distance() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return 0.0f;

        auto& raycast = raycastOpt.value().get();
        return raycast.get_max_distance();
    }

    bool is_hit() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return false;

        auto& raycast = raycastOpt.value().get();
        return raycast.get_is_hit();
    }

    vec3 get_hit_position() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return vec3(0.0f);

        auto& raycast = raycastOpt.value().get();
        return raycast.get_hit_position();
    }

    vec3 get_hit_normal() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return vec3(0.0f);

        auto& raycast = raycastOpt.value().get();
        return raycast.get_hit_normal();
    }

    float get_hit_distance() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return 0.0f;

        auto& raycast = raycastOpt.value().get();
        return raycast.get_hit_distance();
    }

    void set_origin(const vec3& origin) {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return;

        auto& raycast = raycastOpt.value().get();
        raycast.set_origin(origin);
    }

    void set_unit_dir(const vec3& unitDir) {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return;

        auto& raycast = raycastOpt.value().get();
        raycast.set_unit_dir(unitDir);
    }

    void set_max_distance(float maxDistance) {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return;

        auto& raycast = raycastOpt.value().get();
        raycast.set_max_distance(maxDistance);
    }

    void set_raycast(const vec3& origin, const vec3& unitDir, float maxDistance) {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return;

        auto& raycast = raycastOpt.value().get();
        raycast.set_raycast(origin, unitDir, maxDistance);
    }

    JSValue get_hit_actor() {
        auto raycastOpt = get_raycast();
        if (!raycastOpt)
            return JS_UNDEFINED;

        auto& raycast = raycastOpt.value().get();
        PxActor* hit = raycast.get_hit_actor();
        uuid* uuidPtr = reinterpret_cast<uuid*>(hit->userData);

        JSValue obj = create_js_object_from_name("GameObject");
        auto opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));

        (*opaque)->set_uuid(*uuidPtr);

        return obj;
    }
};

class JSClientPlayer : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::ClientPlayer>> get_client_player() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::ClientPlayer>();
    }

    void set_look_axis(vec2 axis) {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return;

        auto& clientPlayer = clientPlayerOpt.value().get();
        clientPlayer.m_lookAxis = axis;
    }

    void set_move_axis(vec2 axis) {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return;

        auto& clientPlayer = clientPlayerOpt.value().get();
        clientPlayer.m_moveAxis = axis;
    }

    void set_jump_pressed(bool pressed) {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return;

        auto& clientPlayer = clientPlayerOpt.value().get();
        clientPlayer.m_jumpPressed = pressed;
    }

    void set_is_shooting(bool shooting) {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return;

        auto& clientPlayer = clientPlayerOpt.value().get();
        clientPlayer.m_isShooting = shooting;
    }

    vec2 get_look_axis() {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return vec2(0.0f, 0.0f);

        auto& clientPlayer = clientPlayerOpt.value().get();
        return clientPlayer.m_lookAxis;
    }

    vec2 get_move_axis() {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return vec2(0.0f, 0.0f);

        auto& clientPlayer = clientPlayerOpt.value().get();
        return clientPlayer.m_moveAxis;
    }

    bool get_jump_pressed() {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return false;

        auto& clientPlayer = clientPlayerOpt.value().get();
        return clientPlayer.m_jumpPressed;
    }

    bool get_is_shooting() {
        auto clientPlayerOpt = get_client_player();
        if (!clientPlayerOpt)
            return false;

        auto& clientPlayer = clientPlayerOpt.value().get();
        return clientPlayer.m_isShooting;
    }
};

class JSParticles : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Particles>> get_particles() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Particles>();
    }
    vec3 get_position() {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return vec3(0, 0, 0);
        auto& particles = particlesOpt.value().get();
        return particles.get_position();
    }
    vec3 get_lookat() {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return vec3(0, 0, 0);
        auto& particles = particlesOpt.value().get();
        return particles.get_lookat();
    }
    float get_paticles_per_second() {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return 0;
        auto& particles = particlesOpt.value().get();
        particles.get_particles_per_second();
    };
    void set_position(vec3 position) {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return;
        auto& particles = particlesOpt.value().get();
        particles.set_position(position);
    }
    void set_lookat(vec3 lookat) {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return;
        auto& particles = particlesOpt.value().get();
        particles.set_lookat(lookat);
    }
    void set_particles_per_second(float rate) {
        auto particlesOpt = get_particles();
        if (!particlesOpt)
            return;
        auto& particles = particlesOpt.value().get();
        particles.set_particles_per_second(rate);
    }
};

class JSCollisionDetection : public JSObjectBase {
   public:
    std::optional<std::reference_wrapper<components::Collision_Detection>> get_CollisionDetection() {
        auto gameObjectOpt = retrieve_game_object();
        if (!gameObjectOpt)
            return std::nullopt;

        return gameObjectOpt.value().get_component<components::Collision_Detection>();
    }
    std::vector<actor_contact_data> get_actor_contact_data() {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<actor_contact_data> acd;
            return acd;
        }
        auto& detection = detectionOpt.value().get();
        return detection.get_actor_contact_data();
    }
    std::vector<name_contact_data> get_name_contact_data() {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<name_contact_data> ncd;
            return ncd;
        }
        auto& detection = detectionOpt.value().get();
        return detection.get_name_contact_data();
    }
    std::vector<contact_data> get_contact_data_by_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return cd;
        }
        auto& detection = detectionOpt.value().get();
        return detection.get_contact_data_by_actor(actor);
    }

    std::vector<contact_data> get_contact_data_by_name(std::string name) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return cd;
        }
        auto& detection = detectionOpt.value().get();
        return detection.get_contact_data_by_name(name);
    }

    void add_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return;
        }
        auto& detection = detectionOpt.value().get();
        detection.add_search_actor(actor);
    }

    void add_search_name(std::string name) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return;
        }
        auto& detection = detectionOpt.value().get();
        detection.add_search_name(name);
    }
    void remove_search_actor(std::shared_ptr<PxDynamic_ptr_wrapper> actor) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return;
        }
        auto& detection = detectionOpt.value().get();
        detection.remove_search_actor(actor);
    }
    void remove_search_actor(std::shared_ptr<PxStatic_ptr_wrapper> actor) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return;
        }
        auto& detection = detectionOpt.value().get();
        detection.remove_search_actor(actor);
    }
    void remove_search_name(std::string name) {
        auto detectionOpt = get_CollisionDetection();
        if (!detectionOpt) {
            std::vector<contact_data> cd;
            return;
        }
        auto& detection = detectionOpt.value().get();
        detection.remove_search_name(name);
    }
};

void script_storage_store(std::string key, qjs::Value value) {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return;

    auto& engine = engineOpt.value().get();
    auto weakScripting = engine.get_scripting_module();
    if (weakScripting.expired())
        return;
    auto scripting = weakScripting.lock();

    log::debug("Storing key: {}", key);

    scripting->storage[key] = value.release();
}

JSValue script_storage_retrieve(std::string key) {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return JS_UNDEFINED;

    auto& engine = engineOpt.value().get();
    auto weakScripting = engine.get_scripting_module();
    if (weakScripting.expired())
        return JS_UNDEFINED;
    auto scripting = weakScripting.lock();

    log::debug("Retrieving key: {}", key);

    if (scripting->storage.count(key))
        return scripting->storage[key];

    log::debug("Failed to retrieve key: {}", key);

    return JS_UNDEFINED;
}

JSValue JSGameObject::add_component(const std::string& name) {
    auto gameObjectOpt = retrieve_game_object();
    if (!gameObjectOpt)
        return JS_UNDEFINED;
    auto gameObject = gameObjectOpt.value();

    // WARNING: Please, if someone has a nice metaprogramming way to do this, let me know.
    if (name == "aggregate") {
        if (!gameObject.has_component<components::Aggregate>()) {
            gameObject.add_component<components::Aggregate>();
        }
    } else if (name == "audioListener") {
        if (!gameObject.has_component<components::AudioListener>()) {
            gameObject.add_component<components::AudioListener>();
        }
    } else if (name == "audioSource") {
        if (!gameObject.has_component<components::AudioSource>()) {
            gameObject.add_component<components::AudioSource>();
        }
    } else if (name == "editorCamera" || name == "camera") {
        if (!gameObject.has_component<components::EditorCamera>()) {
            gameObject.add_component<components::EditorCamera>();
        }
    } else if (name == "clientPlayer") {
        if (!gameObject.has_component<components::ClientPlayer>()) {
            gameObject.add_component<components::ClientPlayer>();
        }
    } else if (name == "instanceMesh" || name == "mesh") {
        if (!gameObject.has_component<components::InstanceMesh>()) {
            gameObject.add_component<components::InstanceMesh>();
        }
    } else if (name == "instanceScript" || name == "script") {
        if (!gameObject.has_component<components::InstanceScript>()) {
            gameObject.add_component<components::InstanceScript>();
        }
    } else if (name == "material") {
        if (!gameObject.has_component<components::Material>()) {
            gameObject.add_component<components::Material>();
        }
    } else if (name == "particles") {
        if (!gameObject.has_component<components::Particles>()) {
            gameObject.add_component<components::Particles>();
        }
    } else if (name == "raycast" || name == "rayCast") {
        if (!gameObject.has_component<components::Raycast>()) {
            gameObject.add_component<components::Raycast>();
        }
    } else if (name == "rigidBody") {
        if (!gameObject.has_component<components::RigidBody>()) {
            gameObject.add_component<components::RigidBody>();
        }
    } else if (name == "spotLight") {
        if (!gameObject.has_component<components::SpotLight>()) {
            gameObject.add_component<components::SpotLight>();
        }
    } else if (name == "texture") {
        if (!gameObject.has_component<components::Texture>()) {
            gameObject.add_component<components::Texture>();
        }
    } else if (name == "transform") {
        if (!gameObject.has_component<components::Transform>()) {
            gameObject.add_component<components::Transform>();
        }
    } else if (name == "name") {
        if (!gameObject.has_component<components::Name>()) {
            gameObject.add_component<components::Name>();
        }
    } else if (name == "tag") {
        if (!gameObject.has_component<components::Tag>()) {
            gameObject.add_component<components::Tag>();
        }
    } else if (name == "hierarchy") {
        if (!gameObject.has_component<components::Hierarchy>()) {
            gameObject.add_component<components::Hierarchy>();
        }
    } else if (name == "collisionDetction") {
        if (gameObject.has_component<components::Collision_Detection>()) {
            gameObject.add_component<components::Collision_Detection>();
        }
    }

    return get_component(name);
}

void JSGameObject::remove_component(const std::string& name) {
    auto gameObjectOpt = retrieve_game_object();
    if (!gameObjectOpt)
        return;
    auto gameObject = gameObjectOpt.value();

    // WARNING: Please, if someone has a nice metaprogramming way to do this, let me know.
    if (name == "aggregate") {
        if (gameObject.has_component<components::Aggregate>()) {
            gameObject.remove_component<components::Aggregate>();
        }
    } else if (name == "audioListener") {
        if (gameObject.has_component<components::AudioListener>()) {
            gameObject.remove_component<components::AudioListener>();
        }
    } else if (name == "audioSource") {
        if (gameObject.has_component<components::AudioSource>()) {
            gameObject.remove_component<components::AudioSource>();
        }
    } else if (name == "editorCamera" || name == "camera") {
        if (gameObject.has_component<components::EditorCamera>()) {
            gameObject.remove_component<components::EditorCamera>();
        }
    } else if (name == "clientPlayer") {
        if (gameObject.has_component<components::ClientPlayer>()) {
            gameObject.remove_component<components::ClientPlayer>();
        }
    } else if (name == "instanceMesh" || name == "mesh") {
        if (gameObject.has_component<components::InstanceMesh>()) {
            gameObject.remove_component<components::InstanceMesh>();
        }
    } else if (name == "instanceScript" || name == "script") {
        if (gameObject.has_component<components::InstanceScript>()) {
            gameObject.remove_component<components::InstanceScript>();
        }
    } else if (name == "material") {
        if (gameObject.has_component<components::Material>()) {
            gameObject.remove_component<components::Material>();
        }
    } else if (name == "particles") {
        if (gameObject.has_component<components::Particles>()) {
            gameObject.remove_component<components::Particles>();
        }
    } else if (name == "raycast" || name == "rayCast") {
        if (gameObject.has_component<components::Raycast>()) {
            gameObject.remove_component<components::Raycast>();
        }
    } else if (name == "rigidBody") {
        if (gameObject.has_component<components::RigidBody>()) {
            gameObject.remove_component<components::RigidBody>();
        }
    } else if (name == "spotLight") {
        if (gameObject.has_component<components::SpotLight>()) {
            gameObject.remove_component<components::SpotLight>();
        }
    } else if (name == "texture") {
        if (gameObject.has_component<components::Texture>()) {
            gameObject.remove_component<components::Texture>();
        }
    } else if (name == "transform") {
        if (gameObject.has_component<components::Transform>()) {
            gameObject.remove_component<components::Transform>();
        }
    } else if (name == "name") {
        if (gameObject.has_component<components::Name>()) {
            gameObject.remove_component<components::Name>();
        }
    } else if (name == "tag") {
        if (gameObject.has_component<components::Tag>()) {
            gameObject.remove_component<components::Tag>();
        }
    } else if (name == "hierarchy") {
        if (gameObject.has_component<components::Hierarchy>()) {
            gameObject.remove_component<components::Hierarchy>();
        }
    } else if (name == "collisionDetction") {
        if (gameObject.has_component<components::Collision_Detection>()) {
            gameObject.remove_component<components::Collision_Detection>();
        }
    }
}

JSValue JSGameObject::get_component(const std::string& name) {
    auto gameObjectOpt = retrieve_game_object();
    if (!gameObjectOpt)
        return JS_UNDEFINED;

    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return JS_UNDEFINED;

    auto& engine = engineOpt.value().get();
    auto weakScripting = engine.get_scripting_module();
    if (weakScripting.expired())
        return JS_UNDEFINED;
    auto scripting = weakScripting.lock();

    if (!scripting->knoting_module)
        return JS_UNDEFINED;

    auto module = scripting->knoting_module->get().get_module();
    if (!module)
        return JS_UNDEFINED;

    // WARNING: Please, if someone has a nice metaprogramming way to do this, let me know.

    std::optional<qjs::Value> ctor;
    std::string toFindExport = "";

    if (name == "aggregate") {
        toFindExport = "Aggregate";
    } else if (name == "audioListener") {
        toFindExport = "AudioListener";
    } else if (name == "audioSource") {
        toFindExport = "AudioSource";
    } else if (name == "editorCamera" || name == "camera") {
        toFindExport = "EditorCamera";
    } else if (name == "clientPlayer") {
        toFindExport = "ClientPlayer";
    } else if (name == "instanceMesh" || name == "mesh") {
        toFindExport = "Mesh";
    } else if (name == "instanceScript" || name == "script") {
        toFindExport = "Script";
    } else if (name == "material") {
        toFindExport = "Material";
    } else if (name == "particles") {
        toFindExport = "Particles";
    } else if (name == "raycast" || name == "rayCast") {
        toFindExport = "Raycast";
    } else if (name == "rigidBody") {
        toFindExport = "RigidBody";
    } else if (name == "spotLight") {
        toFindExport = "SpotLight";
    } else if (name == "texture") {
        toFindExport = "Texture";
    } else if (name == "transform") {
        toFindExport = "Transform";
    } else if (name == "name") {
        toFindExport = "Name";
    } else if (name == "tag") {
        toFindExport = "Tag";
    } else if (name == "hierarchy") {
        toFindExport = "Hierarchy";
    } else if (name == "collisionDetection") {
        toFindExport = "CollisionDetection";
    }

    JSValue obj = create_js_object_from_name(toFindExport);
    std::shared_ptr<JSObjectBase>* opaque = nullptr;

    if (name == "aggregate") {
    } else if (name == "audioListener") {
    } else if (name == "audioSource") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSAudioSource>>::QJSClassId));
    } else if (name == "editorCamera" || name == "camera") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSEditorCamera>>::QJSClassId));
    } else if (name == "clientPlayer") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSClientPlayer>>::QJSClassId));
    } else if (name == "instanceMesh" || name == "mesh") {
    } else if (name == "instanceScript" || name == "script") {
    } else if (name == "material") {
    } else if (name == "particles") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSParticles>>::QJSClassId));
    } else if (name == "raycast" || name == "rayCast") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSRaycast>>::QJSClassId));
    } else if (name == "rigidBody") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSRigidBody>>::QJSClassId));
    } else if (name == "spotLight") {
    } else if (name == "texture") {
    } else if (name == "transform") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSTransform>>::QJSClassId));
    } else if (name == "name") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSName>>::QJSClassId));
    } else if (name == "tag") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSTag>>::QJSClassId));
    } else if (name == "hierarchy") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSHierarchy>>::QJSClassId));
    } else if (name == "conllisionDetection") {
        opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
            JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSCollisionDetection>>::QJSClassId));
    }

    if (!opaque) {
        JS_ThrowInternalError(scripting->get_script_context()->ctx, "Component %s not valid, invalid opaque",
                              name.c_str());
        return JS_UNDEFINED;
    }

    (*opaque)->set_uuid(m_id);

    return obj;
}

JSValue create_game_object(std::string name) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return JS_UNDEFINED;
    }
    auto& scene = sceneOpt.value().get();

    JSValue obj = create_js_object_from_name("GameObject");
    std::shared_ptr<JSObjectBase>* opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
        JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));

    auto gameObject = scene.create_game_object(name);
    (*opaque)->set_uuid(gameObject.get_id());

    return obj;
}

JSValue creat_bullet(bool is_teamA, vec3 spawnPos) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return JS_UNDEFINED;
    }
    auto& scene = sceneOpt.value().get();

    JSValue obj = create_js_object_from_name("GameObject");
    std::shared_ptr<JSObjectBase>* opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
        JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));

    auto gameObject = scene.create_bullet(is_teamA, spawnPos);
    (*opaque)->set_uuid(gameObject.get_id());

    return obj;
}

void remove_game_object(uuid id) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return;
    }
    auto& scene = sceneOpt.value().get();

    auto gameObjectOpt = scene.get_game_object_from_id(id);
    if (!gameObjectOpt) {
        return;
    }

    scene.remove_game_object(gameObjectOpt.value());
}

JSValue get_game_object_from_id(uuid id) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return JS_UNDEFINED;
    }
    auto& scene = sceneOpt.value().get();

    auto gameObjectOpt = scene.get_game_object_from_id(id);
    if (!gameObjectOpt) {
        return JS_UNDEFINED;
    }

    JSValue obj = create_js_object_from_name("GameObject");
    auto opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
        JS_GetOpaque(obj, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));

    (*opaque)->set_uuid(gameObjectOpt.value().get_id());

    return obj;
}

std::vector<uuid> find_game_object(const std::string& name) {
    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt) {
        return {};
    }
    auto& scene = sceneOpt.value().get();

    auto& entities = scene.get_game_objects();

    std::vector<uuid> result;
    for (auto [id, gameObject] : entities) {
        auto& nameComponent = gameObject.get_component<components::Name>();
        if (nameComponent.name == name)
            result.push_back(id);
    }

    return result;
}

void load_scene(const std::string& filename) {
    std::filesystem::path path = AssetManager::get_resources_path().append(filename);
    std::fstream serializedSceneStream(path);

    if (!std::filesystem::exists(path)) {
        js_throw_exception("Scene file `%s` not found", filename.c_str());
        return;
    }

    Scene loadedScene;
    Scene::set_active_scene(loadedScene);
    serializedSceneStream.open(path, std::ios_base::in);
    loadedScene.load_scene_from_stream(serializedSceneStream);
    serializedSceneStream.close();
}

void Scripting::add_knoting_module() {
    knoting_module = m_context->addModule("knoting");
    auto& knoting = knoting_module.value().get();

    knoting.class_<JSGameObject>("GameObject")
        .constructor()
        .fun<&JSGameObject::get_id>("getID")
        .fun<&JSGameObject::get_handle>("getHandle")
        .fun<&JSGameObject::has_component>("hasComponent")
        .fun<&JSGameObject::has_no_components>("hasNoComponents")
        .fun<&JSGameObject::get_component>("getComponent")
        .fun<&JSGameObject::remove_component>("removeComponent")
        .fun<&JSGameObject::add_component>("addComponent");

    knoting.class_<JSTransform>("Transform")
        .constructor()
        .fun<&JSTransform::set_position>("setPosition")
        .fun<&JSTransform::set_scale>("setScale")
        .fun<&JSTransform::set_rotation>("setRotation")
        .fun<&JSTransform::set_rotation_euler>("setRotationEuler")
        .fun<&JSTransform::get_position>("getPosition")
        .fun<&JSTransform::get_scale>("getScale")
        .fun<&JSTransform::get_rotation>("getRotation")
        .fun<&JSTransform::get_rotation_euler>("getRotationEuler")
        .fun<&JSTransform::get_forward>("getForward")
        .fun<&JSTransform::get_right>("getRight")
        .fun<&JSTransform::get_up>("getUp");

    knoting.class_<JSRigidBody>("RigidBody")
        .constructor()
        .fun<&JSRigidBody::get_actor>("getActor")
        .fun<&JSRigidBody::set_rotation>("setRotation")
        .fun<&JSRigidBody::add_force>("addForce")
        .fun<&JSRigidBody::get_position>("getPosition")
        .fun<&JSRigidBody::get_rotation>("getRotation")
        .fun<&JSRigidBody::get_rotation_euler>("getRotationEuler")
        .fun<&JSRigidBody::get_name>("getName")
        .fun<&JSRigidBody::set_name>("setName")
        .fun<&JSRigidBody::get_mass>("getMass")
        .fun<&JSRigidBody::is_dynamic>("isDynamic")
        .fun<&JSRigidBody::is_kinematic>("isKinematic")
        .fun<&JSRigidBody::get_angular_damping>("getAngularDamping")
        .fun<&JSRigidBody::get_linear_damping>("getLinearDamping")
        .fun<&JSRigidBody::get_angular_velocity>("getAngularVelocity")
        .fun<&JSRigidBody::get_linear_velocity>("getLinearVelocity")
        .fun<&JSRigidBody::set_kinematic>("setKinematic")
        .fun<&JSRigidBody::set_mass>("setMass")
        .fun<&JSRigidBody::set_angular_damping>("setAngularDamping")
        .fun<&JSRigidBody::set_linear_damping>("setLinearDamping")
        .fun<&JSRigidBody::set_angular_velocity>("setAngularVelocity")
        .fun<&JSRigidBody::set_linear_velocity>("setLinearVelocity")
        .fun<&JSRigidBody::add_torque>("addTorque")
        .fun<&JSRigidBody::clear_force>("clearForce")
        .fun<&JSRigidBody::clear_torque>("clearTorque");

    knoting.class_<JSHierarchy>("Hierarchy")
        .constructor()
        .fun<&JSHierarchy::get_parent>("getParent")
        .fun<&JSHierarchy::has_parent>("hasParent")
        .fun<&JSHierarchy::set_parent>("setParent")
        .fun<&JSHierarchy::has_children>("hasChildren")
        .fun<&JSHierarchy::has_child>("hasChild")
        .fun<&JSHierarchy::get_children>("getChildren")
        .fun<&JSHierarchy::add_child>("addChild")
        .fun<&JSHierarchy::remove_child>("removeChild");

    knoting.class_<JSName>("Name").constructor().fun<&JSName::set_name>("setName").fun<&JSName::get_name>("getName");

    knoting.class_<JSTag>("Tag")
        .constructor()
        .fun<&JSTag::get_tag_name>("getTag")
        .fun<&JSTag::set_tag_name>("setTag")
        .fun<&JSTag::registerTag>("registerTag")
        .fun<&JSTag::unregisterTag>("unregisterTag")
        .fun<&JSTag::get_registered_tags>("getRegisteredTags");

    knoting.class_<JSEditorCamera>("EditorCamera")
        .constructor()
        .fun<&JSEditorCamera::set_look_target>("setLookTarget")
        .fun<&JSEditorCamera::set_fov>("setFov")
        .fun<&JSEditorCamera::set_z_near>("setZNear")
        .fun<&JSEditorCamera::set_z_far>("setZFar")
        .fun<&JSEditorCamera::set_move_speed>("setMoveSpeed")
        .fun<&JSEditorCamera::get_up>("getUP")
        .fun<&JSEditorCamera::get_look_target>("getLookTarget")
        .fun<&JSEditorCamera::get_fov>("getFov")
        .fun<&JSEditorCamera::get_z_near>("getZNear")
        .fun<&JSEditorCamera::get_z_far>("getZFar")
        .fun<&JSEditorCamera::get_move_speed>("getMoveSpeed");

    knoting.class_<JSClientPlayer>("ClientPlayer")
        .constructor()
        .fun<&JSClientPlayer::set_look_axis>("setLookAxis")
        .fun<&JSClientPlayer::set_move_axis>("setMoveAxis")
        .fun<&JSClientPlayer::set_is_shooting>("setIsShooting")
        .fun<&JSClientPlayer::JSClientPlayer::set_jump_pressed>("setJumpPressed")
        .fun<&JSClientPlayer::get_look_axis>("getLookAxis")
        .fun<&JSClientPlayer::get_move_axis>("getMoveAxis")
        .fun<&JSClientPlayer::get_is_shooting>("getIsShooting")
        .fun<&JSClientPlayer::JSClientPlayer::get_jump_pressed>("getJumpPressed");

    knoting.class_<JSParticles>("Particles")
        .constructor()
        .fun<&JSParticles::set_particles_per_second>("setParticlesPerSecond")
        .fun<&JSParticles::set_position>("setPosition")
        .fun<&JSParticles::set_lookat>("setLookat")
        .fun<&JSParticles::get_paticles_per_second>("getParticlesPerSecond")
        .fun<&JSParticles::get_position>("getPosition")
        .fun<&JSParticles::get_lookat>("getLookat");

    knoting.class_<JSCollisionDetection>("ConllisionDetection")
        .constructor()
        .fun<&JSCollisionDetection::get_name_contact_data>("getNameContactData")
        .fun<&JSCollisionDetection::get_contact_data_by_actor>("getContactDataByActor")
        .fun<&JSCollisionDetection::get_contact_data_by_actor>("getContactDataByActor")
        .fun<&JSCollisionDetection::get_contact_data_by_name>("getContactDataByName")
        .fun<&JSCollisionDetection::get_actor_contact_data>("getActorContactData")
        .fun<&JSCollisionDetection::add_search_actor>("addSearchActor")
        .fun<&JSCollisionDetection::add_search_actor>("addSearchActor")
        .fun<&JSCollisionDetection::add_search_name>("addSearchName")
        .fun<&JSCollisionDetection::remove_search_actor>("removeSearchActor")
        .fun<&JSCollisionDetection::remove_search_actor>("removeSearchActor")
        .fun<&JSCollisionDetection::remove_search_name>("removeSearchName");

    auto storage = m_context->newObject();
    storage.add("store", script_storage_store);
    storage.add("retrieve", script_storage_retrieve);

    knoting.add("storage", storage);

    auto input = m_context->newObject();
    input.add("keyPressed", input_key_pressed);
    input.add("keyHeldDown", input_key_held_down);
    input.add("keyOnTrigger", input_key_on_trigger);
    input.add("keyOnRelease", input_key_on_release);

    input.add("padButtonPressed", input_pad_button_pressed);
    input.add("padButtonHeldDown", input_pad_button_held_down);
    input.add("padButtonTriggered", input_pad_button_triggered);
    input.add("padButtonReleased", input_pad_button_released);
    input.add("getPadAxis", input_get_pad_axis);
    input.add("padPresent", input_pad_present);

    input.add("mouseButtonPressed", input_mouse_button_pressed);
    input.add("mouseButtonHeldDown", input_mouse_button_held_down);
    input.add("mouseButtonTriggered", input_mouse_button_triggered);
    input.add("mouseButtonReleased", input_mouse_button_released);
    input.add("mouseDoubleClicked", input_mouse_double_clicked);
    input.add("wheelMoved", input_wheel_moved);
    input.add("getRelativePosition", input_get_relative_position);
    input.add("getAbsolutePosition", input_get_absolute_position);
    input.add("getWheelMovement", input_get_wheel_movement);
    input.add("setMouseSensitivity", input_set_mouse_sensitivity);
    input.add("setDoubleClickLimit", input_set_double_click_limit);

    knoting.add("input", input);

    auto scene = m_context->newObject();
    scene.add("createGameObject", create_game_object);
    scene.add("createBullet", creat_bullet);
    scene.add("removeGameObject", remove_game_object);
    scene.add("getGameObjectFromID", get_game_object_from_id);
    scene.add("findGameObject", find_game_object);
    scene.add("loadScene", load_scene);

    knoting.add("scene", scene);

    auto mouseButtonCode = m_context->newObject();
    mouseButtonCode["Left"] = static_cast<int>(MouseButtonCode::Left);
    mouseButtonCode["Right"] = static_cast<int>(MouseButtonCode::Right);
    mouseButtonCode["Middle"] = static_cast<int>(MouseButtonCode::Middle);
    mouseButtonCode["Button3"] = static_cast<int>(MouseButtonCode::Button3);
    mouseButtonCode["Button4"] = static_cast<int>(MouseButtonCode::Button4);
    mouseButtonCode["Button5"] = static_cast<int>(MouseButtonCode::Button5);
    mouseButtonCode["Button6"] = static_cast<int>(MouseButtonCode::Button6);
    mouseButtonCode["Button7"] = static_cast<int>(MouseButtonCode::Button7);
    mouseButtonCode["Button8"] = static_cast<int>(MouseButtonCode::Button8);
    mouseButtonCode["Last"] = static_cast<int>(MouseButtonCode::Last);

    knoting.add("MouseButtonCode", mouseButtonCode);

    auto joyStickCode = m_context->newObject();
    joyStickCode["LeftX"] = static_cast<int>(JoyStickCode::LeftX);
    joyStickCode["LeftY"] = static_cast<int>(JoyStickCode::LeftY);
    joyStickCode["RightX"] = static_cast<int>(JoyStickCode::RightX);
    joyStickCode["RightY"] = static_cast<int>(JoyStickCode::RightY);
    joyStickCode["LeftBumper"] = static_cast<int>(JoyStickCode::LeftBumper);
    joyStickCode["RightBumper"] = static_cast<int>(JoyStickCode::RightBumper);
    joyStickCode["Last"] = static_cast<int>(JoyStickCode::Last);

    knoting.add("JoyStickCode", joyStickCode);

    auto padCode = m_context->newObject();
    padCode["Pad0"] = static_cast<int>(PadCode::Pad0);
    padCode["Pad1"] = static_cast<int>(PadCode::Pad1);
    padCode["Pad2"] = static_cast<int>(PadCode::Pad2);
    padCode["Pad3"] = static_cast<int>(PadCode::Pad3);
    padCode["Pad4"] = static_cast<int>(PadCode::Pad4);
    padCode["Pad5"] = static_cast<int>(PadCode::Pad5);
    padCode["Pad6"] = static_cast<int>(PadCode::Pad6);
    padCode["Pad7"] = static_cast<int>(PadCode::Pad7);
    padCode["Pad8"] = static_cast<int>(PadCode::Pad8);
    padCode["Pad9"] = static_cast<int>(PadCode::Pad9);
    padCode["Pad10"] = static_cast<int>(PadCode::Pad10);
    padCode["Pad11"] = static_cast<int>(PadCode::Pad11);
    padCode["Pad12"] = static_cast<int>(PadCode::Pad12);
    padCode["Pad13"] = static_cast<int>(PadCode::Pad13);
    padCode["Pad14"] = static_cast<int>(PadCode::Pad14);
    padCode["Pad15"] = static_cast<int>(PadCode::Pad15);
    padCode["Last"] = static_cast<int>(PadCode::Last);

    knoting.add("PadCode", padCode);

    auto padButtonCode = m_context->newObject();
    padButtonCode["A"] = static_cast<int>(PadButtonCode::A);
    padButtonCode["B"] = static_cast<int>(PadButtonCode::B);
    padButtonCode["X"] = static_cast<int>(PadButtonCode::X);
    padButtonCode["Y"] = static_cast<int>(PadButtonCode::Y);
    padButtonCode["LeftBumper"] = static_cast<int>(PadButtonCode::LeftBumper);
    padButtonCode["RightBumper"] = static_cast<int>(PadButtonCode::RightBumper);
    padButtonCode["Back"] = static_cast<int>(PadButtonCode::Back);
    padButtonCode["Start"] = static_cast<int>(PadButtonCode::Start);
    padButtonCode["Guide"] = static_cast<int>(PadButtonCode::Guide);
    padButtonCode["LeftThumb"] = static_cast<int>(PadButtonCode::LeftThumb);
    padButtonCode["RightThumb"] = static_cast<int>(PadButtonCode::RightThumb);
    padButtonCode["DPadUp"] = static_cast<int>(PadButtonCode::DPadUp);
    padButtonCode["DPadRight"] = static_cast<int>(PadButtonCode::DPadRight);
    padButtonCode["DPadDown"] = static_cast<int>(PadButtonCode::DPadDown);
    padButtonCode["DPadLeft"] = static_cast<int>(PadButtonCode::DPadLeft);
    padButtonCode["Last"] = static_cast<int>(PadButtonCode::Last);

    knoting.add("PadButtonCode", padButtonCode);

    auto keyCode = m_context->newObject();
    keyCode["Unknown"] = static_cast<int>(KeyCode::Unknown);
    keyCode["Space"] = static_cast<int>(KeyCode::Space);
    keyCode["Apostrophe"] = static_cast<int>(KeyCode::Apostrophe);
    keyCode["Comma"] = static_cast<int>(KeyCode::Comma);
    keyCode["Minus"] = static_cast<int>(KeyCode::Minus);
    keyCode["Period"] = static_cast<int>(KeyCode::Period);
    keyCode["Slash"] = static_cast<int>(KeyCode::Slash);
    keyCode["N0"] = static_cast<int>(KeyCode::N0);
    keyCode["N1"] = static_cast<int>(KeyCode::N1);
    keyCode["N2"] = static_cast<int>(KeyCode::N2);
    keyCode["N3"] = static_cast<int>(KeyCode::N3);
    keyCode["N4"] = static_cast<int>(KeyCode::N4);
    keyCode["N5"] = static_cast<int>(KeyCode::N5);
    keyCode["N6"] = static_cast<int>(KeyCode::N6);
    keyCode["N7"] = static_cast<int>(KeyCode::N7);
    keyCode["N8"] = static_cast<int>(KeyCode::N8);
    keyCode["N9"] = static_cast<int>(KeyCode::N9);
    keyCode["Semicolon"] = static_cast<int>(KeyCode::Semicolon);
    keyCode["Equal"] = static_cast<int>(KeyCode::Equal);
    keyCode["A"] = static_cast<int>(KeyCode::A);
    keyCode["B"] = static_cast<int>(KeyCode::B);
    keyCode["C"] = static_cast<int>(KeyCode::C);
    keyCode["D"] = static_cast<int>(KeyCode::D);
    keyCode["E"] = static_cast<int>(KeyCode::E);
    keyCode["F"] = static_cast<int>(KeyCode::F);
    keyCode["G"] = static_cast<int>(KeyCode::G);
    keyCode["H"] = static_cast<int>(KeyCode::H);
    keyCode["I"] = static_cast<int>(KeyCode::I);
    keyCode["J"] = static_cast<int>(KeyCode::J);
    keyCode["K"] = static_cast<int>(KeyCode::K);
    keyCode["L"] = static_cast<int>(KeyCode::L);
    keyCode["M"] = static_cast<int>(KeyCode::M);
    keyCode["N"] = static_cast<int>(KeyCode::N);
    keyCode["O"] = static_cast<int>(KeyCode::O);
    keyCode["P"] = static_cast<int>(KeyCode::P);
    keyCode["Q"] = static_cast<int>(KeyCode::Q);
    keyCode["R"] = static_cast<int>(KeyCode::R);
    keyCode["S"] = static_cast<int>(KeyCode::S);
    keyCode["T"] = static_cast<int>(KeyCode::T);
    keyCode["U"] = static_cast<int>(KeyCode::U);
    keyCode["V"] = static_cast<int>(KeyCode::V);
    keyCode["W"] = static_cast<int>(KeyCode::W);
    keyCode["X"] = static_cast<int>(KeyCode::X);
    keyCode["Y"] = static_cast<int>(KeyCode::Y);
    keyCode["Z"] = static_cast<int>(KeyCode::Z);
    keyCode["LeftBracket"] = static_cast<int>(KeyCode::LeftBracket);
    keyCode["Backslash"] = static_cast<int>(KeyCode::Backslash);
    keyCode["RightBracket"] = static_cast<int>(KeyCode::RightBracket);
    keyCode["GraveAccent"] = static_cast<int>(KeyCode::GraveAccent);
    keyCode["World1"] = static_cast<int>(KeyCode::World1);
    keyCode["World2"] = static_cast<int>(KeyCode::World2);
    keyCode["Escape"] = static_cast<int>(KeyCode::Escape);
    keyCode["Enter"] = static_cast<int>(KeyCode::Enter);
    keyCode["Tab"] = static_cast<int>(KeyCode::Tab);
    keyCode["Backspace"] = static_cast<int>(KeyCode::Backspace);
    keyCode["Insert"] = static_cast<int>(KeyCode::Insert);
    keyCode["Delete"] = static_cast<int>(KeyCode::Delete);
    keyCode["Right"] = static_cast<int>(KeyCode::Right);
    keyCode["Left"] = static_cast<int>(KeyCode::Left);
    keyCode["Down"] = static_cast<int>(KeyCode::Down);
    keyCode["Up"] = static_cast<int>(KeyCode::Up);
    keyCode["PageUp"] = static_cast<int>(KeyCode::PageUp);
    keyCode["PageDown"] = static_cast<int>(KeyCode::PageDown);
    keyCode["Home"] = static_cast<int>(KeyCode::Home);
    keyCode["End"] = static_cast<int>(KeyCode::End);
    keyCode["CapsLock"] = static_cast<int>(KeyCode::CapsLock);
    keyCode["ScrollLock"] = static_cast<int>(KeyCode::ScrollLock);
    keyCode["NumLock"] = static_cast<int>(KeyCode::NumLock);
    keyCode["PrintScreen"] = static_cast<int>(KeyCode::PrintScreen);
    keyCode["Pause"] = static_cast<int>(KeyCode::Pause);
    keyCode["F1"] = static_cast<int>(KeyCode::F1);
    keyCode["F2"] = static_cast<int>(KeyCode::F2);
    keyCode["F3"] = static_cast<int>(KeyCode::F3);
    keyCode["F4"] = static_cast<int>(KeyCode::F4);
    keyCode["F5"] = static_cast<int>(KeyCode::F5);
    keyCode["F6"] = static_cast<int>(KeyCode::F6);
    keyCode["F7"] = static_cast<int>(KeyCode::F7);
    keyCode["F8"] = static_cast<int>(KeyCode::F8);
    keyCode["F9"] = static_cast<int>(KeyCode::F9);
    keyCode["F10"] = static_cast<int>(KeyCode::F10);
    keyCode["F11"] = static_cast<int>(KeyCode::F11);
    keyCode["F12"] = static_cast<int>(KeyCode::F12);
    keyCode["F13"] = static_cast<int>(KeyCode::F13);
    keyCode["F14"] = static_cast<int>(KeyCode::F14);
    keyCode["F15"] = static_cast<int>(KeyCode::F15);
    keyCode["F16"] = static_cast<int>(KeyCode::F16);
    keyCode["F17"] = static_cast<int>(KeyCode::F17);
    keyCode["F18"] = static_cast<int>(KeyCode::F18);
    keyCode["F19"] = static_cast<int>(KeyCode::F19);
    keyCode["F20"] = static_cast<int>(KeyCode::F20);
    keyCode["F21"] = static_cast<int>(KeyCode::F21);
    keyCode["F22"] = static_cast<int>(KeyCode::F22);
    keyCode["F23"] = static_cast<int>(KeyCode::F23);
    keyCode["F24"] = static_cast<int>(KeyCode::F24);
    keyCode["F25"] = static_cast<int>(KeyCode::F25);
    keyCode["KP0"] = static_cast<int>(KeyCode::KP0);
    keyCode["KP1"] = static_cast<int>(KeyCode::KP1);
    keyCode["KP2"] = static_cast<int>(KeyCode::KP2);
    keyCode["KP3"] = static_cast<int>(KeyCode::KP3);
    keyCode["KP4"] = static_cast<int>(KeyCode::KP4);
    keyCode["KP5"] = static_cast<int>(KeyCode::KP5);
    keyCode["KP6"] = static_cast<int>(KeyCode::KP6);
    keyCode["KP7"] = static_cast<int>(KeyCode::KP7);
    keyCode["KP8"] = static_cast<int>(KeyCode::KP8);
    keyCode["KP9"] = static_cast<int>(KeyCode::KP9);
    keyCode["KPDecimal"] = static_cast<int>(KeyCode::KPDecimal);
    keyCode["KPDivide"] = static_cast<int>(KeyCode::KPDivide);
    keyCode["KPMultiply"] = static_cast<int>(KeyCode::KPMultiply);
    keyCode["KPSubtract"] = static_cast<int>(KeyCode::KPSubtract);
    keyCode["KPAdd"] = static_cast<int>(KeyCode::KPAdd);
    keyCode["KPEnter"] = static_cast<int>(KeyCode::KPEnter);
    keyCode["KPEqual"] = static_cast<int>(KeyCode::KPEqual);
    keyCode["LeftShift"] = static_cast<int>(KeyCode::LeftShift);
    keyCode["LeftControl"] = static_cast<int>(KeyCode::LeftControl);
    keyCode["LeftAlt"] = static_cast<int>(KeyCode::LeftAlt);
    keyCode["LeftSuper"] = static_cast<int>(KeyCode::LeftSuper);
    keyCode["RightShift"] = static_cast<int>(KeyCode::RightShift);
    keyCode["RightControl"] = static_cast<int>(KeyCode::RightControl);
    keyCode["RightAlt"] = static_cast<int>(KeyCode::RightAlt);
    keyCode["RightSuper"] = static_cast<int>(KeyCode::RightSuper);
    keyCode["Menu"] = static_cast<int>(KeyCode::Menu);

    knoting.add("KeyCode", keyCode);
}

}  // namespace knot
