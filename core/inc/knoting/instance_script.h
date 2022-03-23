#pragma once

#include <knoting/script.h>
#include <knoting/scripting.h>
#include <knoting/assert.h>
#include <knoting/component.h>
#include <knoting/types.h>
#include <knoting/quickjs.hpp>

#include <quickjs-libc.h>

namespace knot {
namespace components {

enum class DefaultScriptFunction {
    Awake,
    Update,
    LateUpdate,
    Destroy,
    OnCollision,
    OnCollisionEnter,
    OnCollisionExit,
    LAST
};

class InstanceScript : public Component<InstanceScript> {
   public:
    InstanceScript();
    InstanceScript(const std::string& path);
    ~InstanceScript();

    void on_awake() override;
    void on_destroy() override;

    std::weak_ptr<components::Script> get_script();
    std::shared_ptr<Scripting> get_scripting();

    qjs::Value get_function(const std::string& function);

    template <typename... Args>
    qjs::Value call_function(DefaultScriptFunction function, Args&&... args) {
        if (!m_defaultFunctions[static_cast<int>(function)].second) {
            return JS_UNDEFINED;
        }

        auto scripting = get_scripting();
        if (!scripting) {
            return JS_UNDEFINED;
        }
        auto context = scripting->get_script_context();

        qjs::Value functionDup =
            context->newValue(JS_DupValue(context->ctx, m_defaultFunctions[static_cast<size_t>(function)].first));

        if (JS_IsObject(functionDup.v) && !JS_IsFunction(context->ctx, functionDup.v)) {
            KNOTING_ASSERT_MESSAGE(false, "Failed to call function");
            return JS_UNDEFINED;
        }

        JSValue argv[sizeof...(Args) + 1];
        qjs::detail::wrap_args(context->ctx, argv, std::forward<Args>(args)...);
        qjs::Value ret =
            JS_Call(context->ctx, functionDup.v, obj->v, sizeof...(args), const_cast<JSValueConst*>(argv));

        for (auto& arg : argv)
            JS_FreeValue(context->ctx, arg);

        if (JS_IsException(ret.v)) {
            js_std_dump_error(context->ctx);
            return JS_UNDEFINED;
        }

        return ret;
    }

    std::optional<qjs::Value> obj;

   private:
    std::shared_ptr<components::Script> m_script;
    std::string m_path;

    std::array<std::pair<JSValue, bool>, static_cast<size_t>(DefaultScriptFunction::LAST)> m_defaultFunctions;
};

}  // namespace components
}  // namespace knot
