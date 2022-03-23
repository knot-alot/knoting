#include <knoting/script.h>

#include <knoting/engine.h>
#include <fstream>

#include <quickjs-libc.h>
#include <quickjs.h>

namespace knot {

namespace components {

Script::Script() : Asset(AssetType::Script, "") {}

Script::Script(const std::string& path) : Asset(AssetType::Script, path) {}

void Script::on_awake() {
    if (m_assetState != AssetState::Finished) {
        m_assetState = AssetState::Loading;
        load_module(m_fullPath);
    }
}

qjs::Value Script::eval_buffer(std::shared_ptr<qjs::Context>& context,
                               const char* buffer,
                               size_t bufferLength,
                               const std::filesystem::path& path) {
    qjs::Value ret = context->newValue(JS_Eval(context->ctx, buffer, bufferLength, path.string().c_str(),
                                               JS_EVAL_TYPE_MODULE | JS_EVAL_FLAG_COMPILE_ONLY));
    if (JS_IsException(ret.v)) {
        js_std_dump_error(context->ctx);
        return JS_UNDEFINED;
    }

    return ret;
}

qjs::Value Script::eval_file(std::shared_ptr<qjs::Context>& context, const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        log::error("Script '{}' doesn't exist...", path.string());
        return JS_UNDEFINED;
    }

    std::ifstream file(path);
    std::string script((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    qjs::Value ret = eval_buffer(context, script.data(), script.length(), path);

    file.close();

    return ret;
}

void Script::load_module(const std::string& path) {
    if (path.empty()) {
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

    std::filesystem::path fullPath = AssetManager::get_resources_path().append(PATH_SCRIPTS).append(path);
    auto ret = eval_file(context, fullPath);

    m_module.module = static_cast<JSModuleDef*>(JS_VALUE_GET_PTR(ret.v));
    m_module.result = JS_MKPTR(JS_TAG_MODULE, m_module.module);

    ret.release();

    if (!m_module.module) {
        log::error("Failed to load module '{}'", path);
        m_assetState = AssetState::Failed;
        context->dumpException();
        return;
    }

    qjs::Value evalRet = context->newValue(JS_EvalFunction(context->ctx, m_module.result));
    if (JS_IsException(evalRet.v)) {
        log::error("Failed to eval module '{}'", path);
        context->dumpException();
        m_assetState = AssetState::Failed;
        m_module.module = nullptr;
        return;
    }

    m_assetState = AssetState::Finished;
}

void Script::on_destroy() {
    auto engineOpt = Engine::get_active_engine();
    if (!engineOpt)
        return;

    auto& engine = engineOpt.value().get();
    auto scriptingWeak = engine.get_scripting_module();
    if (scriptingWeak.expired())
        return;

    auto scripting = scriptingWeak.lock();
    auto context = scripting->get_script_context();

    log::info("Removed script module: '{}'", m_fullPath);
}

void Script::generate_default_asset() {
    m_assetState = AssetState::Loading;
    m_fullPath = "fallbackScript";
    m_module.module = nullptr;
    m_module.result = JS_UNDEFINED;
}

}  // namespace components
}  // namespace knot
