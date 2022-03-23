#pragma once

#include <knoting/asset.h>
#include <knoting/quickjs.hpp>

#include <filesystem>

namespace knot {

namespace components {

class ModuleCache {
   public:
    JSModuleDef* module = nullptr;
    JSValue result;
};

class Script : public Asset {
   public:
    Script();
    Script(const std::string& path);

    void on_awake() override;
    void on_destroy() override;

    void generate_default_asset() override;

   protected:
    friend class InstanceScript;

    void load_module(const std::string& path);
    qjs::Value eval_file(std::shared_ptr<qjs::Context>& context, const std::filesystem::path& path);
    qjs::Value eval_buffer(std::shared_ptr<qjs::Context>& context,
                           const char* buf,
                           size_t buf_len,
                           const std::filesystem::path& path);

    ModuleCache m_module;
};

}  // namespace components
}  // namespace knot
