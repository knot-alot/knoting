#include <knoting/engine.h>
#include <knoting/instance_script.h>
#include <knoting/scene.h>
#include <knoting/script.h>

#include <knoting/asset_manager.h>

#include <knoting/quickjs.hpp>

namespace knot {
namespace components {

std::string default_function_name(DefaultScriptFunction function) {
    switch (function) {
        case DefaultScriptFunction::Awake:
            return "awake";
            break;
        case DefaultScriptFunction::Update:
            return "update";
            break;
        case DefaultScriptFunction::LateUpdate:
            return "lateUpdate";
            break;
        case DefaultScriptFunction::Destroy:
            return "destroy";
            break;
        case DefaultScriptFunction::OnCollision:
            return "onCollision";
            break;
        case DefaultScriptFunction::OnCollisionEnter:
            return "onCollisionEnter";
            break;
        case DefaultScriptFunction::OnCollisionExit:
            return "onCollisionExit";
            break;
        case DefaultScriptFunction::LAST:
            return "";
            break;
    }
}

InstanceScript::InstanceScript() {
    m_defaultFunctions.fill(std::pair(JS_NULL, false));
}

InstanceScript::InstanceScript(const std::string& path) : m_path(path) {}

InstanceScript::~InstanceScript() {
    if (!obj.has_value()) {
        return;
    }

    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return;

    auto& engine = engineOpt.value().get();
    auto scriptingWeak = engine.get_scripting_module();
    if (scriptingWeak.expired())
        return;

    auto scripting = scriptingWeak.lock();
    auto context = scripting->get_script_context();

    for (size_t i = 0; i < static_cast<size_t>(DefaultScriptFunction::LAST); ++i) {
        auto& func = m_defaultFunctions[i];
        if (!func.second)
            continue;
    }

    on_destroy();
}

void InstanceScript::on_awake() {
    auto scripting = get_scripting();
    if (!scripting)
        return;

    m_script = AssetManager::load_asset<components::Script>(m_path).lock();
    if (!m_script)
        return;

    auto context = scripting->get_script_context();
    auto module = m_script->m_module;

    if (!module.module) {
        log::error("Failed to load script module: {}", m_path);
        return;
    }

    std::optional<qjs::Value> mainMod;
    for (int i = 0; i < JS_GetModuleExportEntriesCount(module.module); ++i) {
        JSAtom atomExportName = JS_GetModuleExportEntryName(context->ctx, module.module, i);
        std::string exportName = JS_AtomToCString(context->ctx, atomExportName);

        if (exportName == "default") {
            mainMod = context->newValue(JS_GetModuleExportEntry(context->ctx, module.module, i));
            JS_FreeAtom(context->ctx, atomExportName);
            break;
        }

        JS_FreeAtom(context->ctx, atomExportName);
    }

    if (!mainMod || !JS_IsFunction(context->ctx, mainMod->v)) {
        log::error("InstanceScript: Could not find default export in {}", m_path);
        return;
    }

    if (!JS_IsObject(mainMod->v)) {
        log::error("InstanceScript: Default export is not an object in {}", m_path);
        return;
    }

    auto sceneOpt = Scene::get_active_scene();
    if (!sceneOpt)
        return;
    auto& scene = sceneOpt.value().get();

    auto goOpt = GameObject::get_game_object_from_component(*this);
    if (!goOpt)
        return;

    obj = context->newValue(JS_CallConstructor(context->ctx, mainMod->v, 0, nullptr));

    if (JS_IsException(obj->v)) {
        log::error("InstanceScript: Could not create object from {}", m_path);
        context->dumpException();
        return;
    }

    auto opaque = static_cast<std::shared_ptr<JSObjectBase>*>(
        JS_GetOpaque(obj->v, qjs::js_traits<std::shared_ptr<JSGameObject>>::QJSClassId));
    (*opaque)->set_uuid(goOpt.value().get_id());

    for (size_t i = 0; i < static_cast<size_t>(DefaultScriptFunction::LAST); ++i) {
        auto& func = m_defaultFunctions[i];
        func.first = get_function(default_function_name(static_cast<DefaultScriptFunction>(i))).release();
        if (func.first != JS_NULL)
            func.second = true;
    }

    call_function(DefaultScriptFunction::Awake);
}

void InstanceScript::on_destroy() {
    call_function(DefaultScriptFunction::Destroy);
}

std::weak_ptr<components::Script> InstanceScript::get_script() {
    return m_script;
}

std::shared_ptr<Scripting> InstanceScript::get_scripting() {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return {};

    auto& engine = engineOpt.value().get();
    auto scriptingWeak = engine.get_scripting_module();
    if (scriptingWeak.expired())
        return {};

    return scriptingWeak.lock();
}

qjs::Value InstanceScript::get_function(const std::string& function) {
    if (function.empty() || !obj.has_value()) {
        return JS_UNDEFINED;
    }

    auto scripting = get_scripting();
    if (!scripting)
        return JS_UNDEFINED;
    auto context = scripting->get_script_context();

    qjs::Value ret = context->newValue(JS_GetPropertyStr(context->ctx, obj.value().v, function.c_str()));

    if (!JS_IsFunction(context->ctx, ret.v))
        return JS_NULL;

    return ret;
}

}  // namespace components
}  // namespace knot
