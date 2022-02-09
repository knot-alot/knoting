#pragma once
#include <knoting/assert.h>
#include <knoting/asset.h>
#include <knoting/log.h>
#include <knoting/subsystem.h>
#include <knoting/types.h>
#include <uuid.h>

#include <knoting/mesh.h>
#include <knoting/shader_program.h>
#include <knoting/texture.h>

namespace knot {
using namespace asset;

class AssetManager : public Subsystem {
   public:

    void on_awake() override;
    void on_destroy() override;

    void load_assets_manual();
    void load_assets_serialize();

    template <typename T, typename... TArgs>
    inline std::weak_ptr<T> load_asset(const std::string& path) {
        if (!std::is_base_of<Asset, T>::value) {
            log::error("ASSET : " + path + " IS NOT OF BASE CLASS ASSET");
        }
        auto iterator = m_assets.find(path);
        if (iterator == m_assets.end()) {
            log::debug("Asset : " + path + " is being created");
            m_assets[path] = std::make_shared<T>(path);
        }
        return std::static_pointer_cast<T>(m_assets[path]);
    }

   private:
    //TODO consider tuple / UUID for when assets are loaded via serialization
    std::map<std::string, std::shared_ptr<Asset>> m_assets;
};
}  // namespace knot
